#ifndef REFLANG_SERIALIZER_FUNCTION_HPP
#define REFLANG_SERIALIZER_FUNCTION_HPP

#include <iostream>

#include "serializer.hpp"
#include "types.hpp"

namespace reflang
{
	namespace serializer
	{
		void SerializeFunction(std::ostream& o, const Function& c);
	}
}

#endif //REFLANG_SERIALIZER_FUNCTION_HPP
