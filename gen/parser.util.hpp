#ifndef REFLANG_PARSER_UTIL_HPP
#define REFLANG_PARSER_UTIL_HPP

#include <string>

#include <clang-c/Index.h>

namespace reflang
{
	namespace parser
	{
		std::string Convert(const CXString& s);
		std::string GetFullName(CXCursor cursor);
		std::string GetName(const CXType& type);
		std::string GetFile(const CXCursor& cursor);
	}
}

#endif //REFLANG_PARSER_UTIL_HPP

