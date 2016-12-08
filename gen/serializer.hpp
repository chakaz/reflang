#ifndef REFLANG_SERIALIZER_HPP
#define REFLANG_SERIALIZER_HPP

#include <iostream>

#include "types.hpp"

namespace reflang
{
	namespace serializer
	{
		std::ostream& Serialize(std::ostream& o, const TypeBase& type);
	}
}

#endif //REFLANG_SERIALIZER_HPP
