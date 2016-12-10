#include <iostream>
#include <string>

#include "parser.hpp"
#include "serializer.hpp"

using namespace reflang;
using namespace std;

const string CMakeStartArg = "--";
const string ListTypesArg = "--list-types";
const string GenArg = "--gen";

int main(int argc, char *argv[])
{
	if (argc <= 4 || argv[3] != CMakeStartArg)
	{
		cout << "Usage:" << endl;
		cout << argv[0] << " " << ListTypesArg << " <regex> " << CMakeStartArg
			<< " <cmake arguments>" << endl;
		cout << argv[0] << " " << GenArg << " <regex> " << CMakeStartArg
			<< " <cmake arguments>" << endl;
		exit(0);
	}

	string arg = argv[1];
	string regex = argv[2];

	if (arg == ListTypesArg)
	{
		auto names = parser::GetSupportedTypeNames(
				argc - 4, &argv[4], regex);
		for (const auto& it : names)
		{
			cout << it << endl;
		}
	}
	else if (arg == GenArg)
	{
		auto types = parser::GetTypes(argc - 4, &argv[4], regex);
		serializer::Serialize(types);
	}
	else
	{
		cout << "Invalid argument " << arg << endl;
		exit(-1);
	}
}
