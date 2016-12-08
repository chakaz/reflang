#ifndef REFLANG_PARSER_HPP
#define REFLANG_PARSER_HPP

#include <memory>
#include <string>
#include <vector>

#include "types.hpp"

namespace reflang
{
	namespace parser
	{
		std::vector<std::string> GetSupportedTypeNames(
				int argc, char* argv[], const std::string& regex);

		std::vector<std::unique_ptr<TypeBase>> GetTypes(
				int argc, char* argv[], const std::string& regex);
	};
}

#endif //REFLANG_PARSER_HPP
