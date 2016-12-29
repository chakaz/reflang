#include "cmdargs.hpp"

#include <iostream>

using namespace reflang;
using namespace std;

namespace
{
	void IncrementArgcArgv(int& argc, char**& argv)
	{
		--argc;
		++argv;
	}
}

CmdArgs::Exception::Exception(const string& error)
:	error_(error)
{
}

string CmdArgs::Exception::GetError() const
{
	return error_;
}

vector<string> CmdArgs::Consume(int& argc, char**& argv)
{
	vector<string> results;

	while (argc > 0)
	{
		const string arg = *argv;
		if (arg[0] != '-')
		{
			results.push_back(arg);
			IncrementArgcArgv(argc, argv);
			continue;
		}

		if (arg == "--")
		{
			IncrementArgcArgv(argc, argv);
			break;
		}

		auto it = args_.find(arg);
		if (it == args_.end())
		{
			throw Exception(
					"Unknown argument " + arg + " - did you forget '--'?");
		}

		if (argc == 1)
		{
			throw Exception("No value provided for " + arg);
		}

		const string value = argv[1];
		it->second.setter(value);

		// Increment twice - 1 for param name and 1 for param value.
		IncrementArgcArgv(argc, argv);
		IncrementArgcArgv(argc, argv);
	}

	return results;
}

void CmdArgs::PrintHelp() const
{
	for (const auto& arg : args_)
	{
		cout << arg.first << ":\t" << arg.second.help << endl;
	}
}
