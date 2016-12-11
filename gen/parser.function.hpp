#ifndef REFLANG_PARSER_FUNCTION_HPP
#define REFLANG_PARSER_FUNCTION_HPP

#include <clang-c/Index.h>

#include "types.hpp"

namespace reflang
{
	namespace parser
	{
		Function GetFunction(CXCursor cursor);
	}
}

#endif //REFLANG_PARSER_FUNCTION_HPP
