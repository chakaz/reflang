#include "parser.hpp"

#include <iostream>

#include <clang-c/Index.h>

#include "parser.class.hpp"
#include "parser.enum.hpp"
#include "parser.function.hpp"
#include "parser.util.hpp"

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

	struct GetTypesStruct
	{
		vector<unique_ptr<TypeBase>>* types;
		const parser::Options* options;
	};

	CXChildVisitResult GetTypesVisitor(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* data = reinterpret_cast<GetTypesStruct*>(client_data);
		std::unique_ptr<TypeBase> type;
		switch (clang_getCursorKind(cursor))
		{
			case CXCursor_EnumDecl:
				type = std::make_unique<Enum>(parser::GetEnum(cursor));
				break;
			case CXCursor_ClassDecl:
			case CXCursor_StructDecl:
				type = std::make_unique<Class>(parser::GetClass(cursor));
				break;
			case CXCursor_FunctionDecl:
				type = std::make_unique<Function>(parser::GetFunction(cursor));
				break;
			default:
				break;
		}

		const string& name = type->GetFullName();
		if (type
				&& !name.empty()
				&& parser::IsRecursivelyPublic(cursor)
				&& !(name.back() == ':')
				&& regex_match(name, data->options->include)
				&& !regex_match(name, data->options->exclude))
		{
			data->types->push_back(std::move(type));
		}

		return CXChildVisit_Recurse;
	}
}  // namespace

vector<string> parser::GetSupportedTypeNames(
		int argc, char* argv[], const Options& options)
{
	auto types = GetTypes(argc, argv, options);

	vector<string> names;
	names.reserve(types.size());
	for (const auto& type : types)
	{
		names.push_back(type->GetFullName());
	}
	return names;
}

vector<unique_ptr<TypeBase>> parser::GetTypes(
		int argc, char* argv[], const Options& options)
{
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = Parse(index, argc, argv);

	auto cursor = clang_getTranslationUnitCursor(unit);

	vector<unique_ptr<TypeBase>> results;
	GetTypesStruct data = { &results, &options };
	clang_visitChildren(cursor, GetTypesVisitor, &data);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return results;
}
