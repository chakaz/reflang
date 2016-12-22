#ifndef REFLANG_SERIALIZER_CLASS_HPP
#define REFLANG_SERIALIZER_CLASS_HPP

#include <iostream>

#include "serializer.hpp"
#include "types.hpp"

namespace reflang
{
	namespace serializer
	{
		void SerializeClassHeader(std::ostream& o, const Class& c);
		void SerializeClassSources(std::ostream& o, const Class& c);
	}
}

#endif //REFLANG_SERIALIZER_CLASS_HPP
