#include "parser.class.hpp"

#include "parser.util.hpp"

using namespace reflang;
using namespace std;

namespace
{
	CXChildVisitResult VisitClass(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* data = reinterpret_cast<Class::MethodList*>(client_data);
		if (clang_getCursorKind(cursor) == CXCursor_CXXMethod)
		{
			if (clang_getCXXAccessSpecifier(cursor) == CX_CXXPublic)
			{
				auto type = clang_getCursorType(cursor);

				Function f;
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

				data->push_back(f);
			}
		}
		return CXChildVisit_Continue;
	}

	Class::MethodList GetClassMethods(const CXCursor& cursor)
	{
		Class::MethodList result;

		clang_visitChildren(cursor, VisitClass, &result);

		return result;
	}
}

Class parser::GetClass(CXCursor cursor)
{
	Class c(GetFile(cursor), GetFullName(cursor));
	c.Methods = GetClassMethods(cursor);
	return c;
}
