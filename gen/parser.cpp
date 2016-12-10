#include "parser.hpp"

#include <iostream>
#include <regex>

#include <clang-c/Index.h>

#include "parser.util.hpp"
#include "parser.enum.hpp"
#include "parser.class.hpp"

using namespace reflang;
using namespace std;

namespace
{
	ostream& operator<<(ostream& s, const CXString& str)
	{
		s << parser::Convert(str);
		return s;
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
			string name = parser::GetFullName(cursor);
			if (regex_match(name, *tmp->filter))
			{
				tmp->results->push_back(name);
			}
		}
		return CXChildVisit_Recurse;
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
		std::unique_ptr<TypeBase> type;
		if (clang_getCursorKind(cursor) == CXCursor_EnumDecl)
		{
			type = std::make_unique<Enum>(parser::GetEnum(cursor));
		}
		else if (clang_getCursorKind(cursor) == CXCursor_ClassDecl)
		{
			type = std::make_unique<Class>(parser::GetClass(cursor));
		}

		if (type && regex_match(type->GetFullName(), *tmp->filter))
		{
			tmp->types->push_back(std::move(type));
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
