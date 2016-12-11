#include "parser.function.hpp"

#include "parser.util.hpp"

using namespace reflang;
using namespace std;

Function parser::GetFunction(CXCursor cursor)
{
	Function f(GetFile(cursor), GetFullName(cursor));
	auto type = clang_getCursorType(cursor);

	f.Name = parser::Convert(clang_getCursorSpelling(cursor));
	int num_args = clang_Cursor_getNumArguments(cursor);
	for (int i = 0; i < num_args; ++i)
	{
		auto arg_cursor = clang_Cursor_getArgument(cursor, i);
		NamedObject arg;
		arg.Name = parser::Convert(
				clang_getCursorSpelling(arg_cursor));
		if (arg.Name.empty())
		{
			arg.Name = "nameless";
		}
		auto arg_type = clang_getArgType(type, i);
		arg.Type = parser::GetName(arg_type);
		f.Arguments.push_back(arg);
	}

	f.ReturnType = parser::GetName(clang_getResultType(type));
	return f;
}
