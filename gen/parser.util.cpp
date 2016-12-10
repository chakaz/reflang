#include "parser.util.hpp"
using namespace std;
using namespace reflang;

string parser::Convert(const CXString& s)
{
	string result = clang_getCString(s);
	clang_disposeString(s);
	return result;
}

string parser::GetFullName(CXCursor cursor)
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

string parser::GetFile(const CXCursor& cursor)
{
	auto location = clang_getCursorLocation(cursor);
	CXFile file;
	clang_getSpellingLocation(location, &file, nullptr, nullptr, nullptr);
	return Convert(clang_getFileName(file));
}
