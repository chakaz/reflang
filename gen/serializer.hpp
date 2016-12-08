#ifndef REFLANG_SERIALIZER_HPP
#define REFLANG_SERIALIZER_HPP

#include <iostream>

#include "types.hpp"

namespace reflang
{
	namespace serializer
	{
		void Begin(std::ostream& o);

		void Serialize(std::ostream& o, const TypeBase& type);

		void End(std::ostream& o);
	}
}

#endif //REFLANG_SERIALIZER_HPP
