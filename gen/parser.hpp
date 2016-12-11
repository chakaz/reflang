#ifndef REFLANG_PARSER_HPP
#define REFLANG_PARSER_HPP

#include <memory>
#include <string>
#include <vector>
#include <regex>

#include "types.hpp"

namespace reflang
{
	namespace parser
	{
		struct Options
		{
			std::regex include;
			std::regex exclude;
		};

		std::vector<std::string> GetSupportedTypeNames(
				int argc, char* argv[], const Options& options = Options());

		std::vector<std::unique_ptr<TypeBase>> GetTypes(
				int argc, char* argv[], const Options& options = Options());
	};
}

#endif //REFLANG_PARSER_HPP
