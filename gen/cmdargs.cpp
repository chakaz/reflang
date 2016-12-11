#include "cmdargs.hpp"

#include <iostream>

using namespace reflang;
using namespace std;

CmdArgs::Exception::Exception(const string& error)
:	error_(error)
{
}

string CmdArgs::Exception::GetError() const
{
	return error_;
}

int CmdArgs::Consume(int argc, char** argv)
{
	int consumed = 0;

	while (consumed < argc)
	{
		const string arg = argv[consumed];
		if (arg == "--")
		{
			++consumed;
			break;
		}

		auto it = args_.find(arg);
		if (it == args_.end())
		{
			throw Exception(
					"Unknown argument " + arg + " - did you forget '--'?");
		}

		if (consumed+1 >= argc)
		{
			throw Exception("No value provided for " + arg);
		}

		const string value = argv[consumed+1];
		it->second.setter(value);

		consumed += 2;
	}

	return consumed;
}

void CmdArgs::PrintHelp() const
{
	for (const auto& arg : args_)
	{
		cout << arg.first << ":\t" << arg.second.help << endl;
	}
}
