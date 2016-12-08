#include <string>
#include <iostream>
#include <utility>
#include <vector>

#include <clang-c/Index.h>

using namespace std;

string Convert(const CXString& s) {
	string result = clang_getCString(s);
	clang_disposeString(s);
	return result;
}

ostream& operator<<(ostream& s, const CXString& str) {
	s << Convert(str);
	return s;
}

string GetFullyQualifiedName(CXCursor cursor) {
	string name;
	while (clang_isDeclaration(clang_getCursorKind(cursor)) != 0) {
		string cur = Convert(clang_getCursorDisplayName(cursor));
		if (name.empty()) {
			name = cur;
		} else {
			name = cur + "::" + name;
		}
		cursor = clang_getCursorSemanticParent(cursor);
	}

	return name;
}

using EnumValues = vector<pair<string, int>>;

CXChildVisitResult VisitEnum(
		CXCursor cursor, CXCursor parent, CXClientData client_data) {
	if (clang_getCursorKind(cursor) == CXCursor_EnumConstantDecl) {
		string name = Convert(clang_getCursorSpelling(cursor));
		int value = clang_getEnumConstantDeclValue(cursor);
		reinterpret_cast<EnumValues*>(client_data)->emplace_back(name, value);
	}
	return CXChildVisit_Continue;
}

EnumValues GetEnumValues(const CXCursor& cursor) {
	EnumValues result;

	clang_visitChildren(cursor, VisitEnum, &result);

	return result;
}

CXChildVisitResult CursorVisitor(
		CXCursor cursor, CXCursor parent, CXClientData client_data) {
	cout << ">>> Iterating on cursor of type " << cursor.kind << endl;
	if (clang_getCursorKind(cursor) == CXCursor_EnumDecl) {
		cout << ">>>>> enum: " << GetFullyQualifiedName(cursor) << endl;
		for (const auto& it : GetEnumValues(cursor)) {
			cout << "> " << it.first << " = " << it.second << endl;
		}
	}
	return CXChildVisit_Recurse;
}

int main(int argc, char *argv[]) {
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit;
	auto error = clang_parseTranslationUnit2(
			index, 0, argv, argc, 0, 0, CXTranslationUnit_None, &unit);
	if (error != 0) {
		cout << "Unable to parse translation unit. Quitting." << endl;
		exit(-1);
	}

	int diagnostics = clang_getNumDiagnostics(unit);
	if (diagnostics != 0) {
		cout << ">>> Diagnostics:" << endl;
		for (int i = 0; i != diagnostics; ++i) {
			CXDiagnostic diag = clang_getDiagnostic(unit, i);
			cout << ">>>>> " << clang_formatDiagnostic(diag,
					clang_defaultDiagnosticDisplayOptions());
		}
	}

	auto cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor, CursorVisitor, nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return 0;
}
