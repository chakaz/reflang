#include "parser.class.hpp"

#include "parser.util.hpp"

using namespace reflang;
using namespace std;

namespace
{
	Function GetMethodFromCursor(CXCursor cursor)
	{
		auto type = clang_getCursorType(cursor);

		Function f(
				parser::GetFile(cursor), parser::GetFullName(cursor));
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

	NamedObject GetMemberFromCursor(CXCursor cursor)
	{
		NamedObject member;
		member.Name = parser::Convert(clang_getCursorSpelling(cursor));
		member.Type = parser::GetName(clang_getCursorType(cursor));
		return member;
	}

	CXChildVisitResult VisitClass(
			CXCursor cursor, CXCursor parent, CXClientData client_data)
	{
		auto* c = reinterpret_cast<Class*>(client_data);
		if (clang_getCXXAccessSpecifier(cursor) == CX_CXXPublic)
		{
			switch (clang_getCursorKind(cursor))
			{
			case CXCursor_CXXMethod:
				c->Methods.push_back(GetMethodFromCursor(cursor));
				break;
			case CXCursor_FieldDecl:
				c->Members.push_back(GetMemberFromCursor(cursor));
				break;
			default:
				break;
			}
		}
		return CXChildVisit_Continue;
	}

	Class::MethodList GetClassData(CXCursor cursor, Class& c)
	{
		Class::MethodList result;

		clang_visitChildren(cursor, VisitClass, &c);

		return result;
	}
}

Class parser::GetClass(CXCursor cursor)
{
	Class c(GetFile(cursor), GetFullName(cursor));
	c.Methods = GetClassData(cursor, c);
	return c;
}
