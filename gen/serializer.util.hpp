#ifndef REFLANG_SERIALIZER_UTIL_HPP
#define REFLANG_SERIALIZER_UTIL_HPP

#include <string>
#include <utility>

namespace reflang
{
	namespace serializer
	{
		std::string GetNameWithoutColons(std::string name);

		using FromToPair = std::pair<std::string, std::string>;
		std::string ReplaceAll(
				const std::string& text,
				std::initializer_list<FromToPair> from_to_pairs);
	}
}

#endif //REFLANG_SERIALIZER_UTIL_HPP


