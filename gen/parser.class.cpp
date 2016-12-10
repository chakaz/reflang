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
				string name = parser::Convert(clang_getCursorSpelling(cursor));
				data->push_back(std::move(name));
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
