#include "parser.hpp"

#include <iostream>
#include <regex>

#include <clang-c/Index.h>

using namespace reflang;
using namespace std;

namespace
{
	string Convert(const CXString& s)
	{
		string result = clang_getCString(s);
		clang_disposeString(s);
		return result;
	}

	ostream& operator<<(ostream& s, const CXString& str)
	{
		s << Convert(str);
		return s;
	}

	string GetFullName(CXCursor cursor)
	{
		string name;
		while (clang_isDeclaration(clang_getCursorKind(cursor)) != 0)
		{
			string cur = Convert(clang_getCursorDisplayName(cursor));
			if (name.empty())
			{
				name = cur;
			}
			else
			{
				name = cur + "::" + name;
			}
			cursor = clang_getCursorSemanticParent(cursor);
		}

		return name;
	}

	string GetFile(const CXCursor& cursor)
	{
		auto location = clang_getCursorLocation(cursor);
		CXFile file;
		clang_getSpellingLocation(location, &file, nullptr, nullptr, nullptr);
		return Convert(clang_getFileName(file));
	}

	CXTranslationUnit Parse(CXIndex& index, int argc, char* argv[])
	{
		CXTranslationUnit unit;
		auto error = clang_parseTranslationUnit2(
				index, 0, argv, argc, 0, 0, CXTranslationUnit_None, &unit);
		if (error != 0)
		{
			cerr << "Unable to parse translation unit. Quitting." << endl;
			exit(-1);
		}

		auto diagnostics = clang_getNumDiagnostics(unit);
		if (diagnostics != 0)
		{
			cerr << "> Diagnostics:" << endl;
			for (int i = 0; i != diagnostics; ++i)
			{
				auto diag = clang_getDiagnostic(unit, i);
				cerr << ">>> "
					<< clang_formatDiagnostic(
							diag, clang_defaultDiagnosticDisplayOptions());
			}
		}

		return unit;
	}

	struct GetSupportedTypeNamesStruct
	{
		vector<string>* results;
		regex* filter;
	};

	CXChildVisitResult GetSupportedTypeNamesVisitor(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* tmp = reinterpret_cast<GetSupportedTypeNamesStruct*>(client_data);
		if (clang_getCursorKind(cursor) == CXCursor_EnumDecl)
		{
			string name = GetFullName(cursor);
			if (regex_match(name, *tmp->filter))
			{
				tmp->results->push_back(name);
			}
		}
		return CXChildVisit_Recurse;
	}

	CXChildVisitResult VisitEnum(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		if (clang_getCursorKind(cursor) == CXCursor_EnumConstantDecl)
		{
			string name = Convert(clang_getCursorSpelling(cursor));
			int value = clang_getEnumConstantDeclValue(cursor);
			reinterpret_cast<Enum::ValueList*>(client_data)->emplace_back(
					name, value);
		}
		return CXChildVisit_Continue;
	}

	Enum::ValueList GetEnumValues(const CXCursor& cursor)
	{
		Enum::ValueList result;

		clang_visitChildren(cursor, VisitEnum, &result);

		return result;
	}

	struct GetTypesStruct
	{
		vector<unique_ptr<TypeBase>>* types;
		regex* filter;
	};

	CXChildVisitResult GetTypesVisitor(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* tmp = reinterpret_cast<GetTypesStruct*>(client_data);
		if (clang_getCursorKind(cursor) == CXCursor_EnumDecl)
		{
			auto type = make_unique<Enum>(GetFile(cursor), GetFullName(cursor));
			type->Values = GetEnumValues(cursor);
			if (regex_match(type->GetFullName(), *tmp->filter))
			{
				tmp->types->push_back(move(type));
			}
		}
		return CXChildVisit_Recurse;
	}
}  // namespace

vector<string> parser::GetSupportedTypeNames(
		int argc, char* argv[], const string& regex_str)
{
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = Parse(index, argc, argv);

	auto cursor = clang_getTranslationUnitCursor(unit);

	vector<string> results;
	regex filter(regex_str.c_str());
	GetSupportedTypeNamesStruct tmp = { &results, &filter };
	clang_visitChildren(cursor, GetSupportedTypeNamesVisitor, &tmp);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return results;
}

vector<unique_ptr<TypeBase>> parser::GetTypes(
		int argc, char* argv[], const string& regex_str)
{
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = Parse(index, argc, argv);

	auto cursor = clang_getTranslationUnitCursor(unit);

	vector<unique_ptr<TypeBase>> results;
	regex filter(regex_str.c_str());
	GetTypesStruct tmp = { &results, &filter };
	clang_visitChildren(cursor, GetTypesVisitor, &tmp);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return results;
}
