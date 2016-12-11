#include <iostream>
#include <string>

#include "cmdargs.hpp"
#include "parser.hpp"
#include "serializer.hpp"

using namespace reflang;
using namespace std;

int main(int argc, char *argv[])
{
	CmdArgs cmd_args;
	auto list_only = cmd_args.Register<bool>(
			"--list-only",
			"Only list type names, don't generate",
			false);
	auto filter_include = cmd_args.Register<string>(
			"--include",
			"regex for which types to include in reflection generation",
			".*");
	auto filter_exclude = cmd_args.Register<string>(
			"--exclude",
			"regex for which types to exclude from reflection generation",
			"std::.*");

	bool wtf = false;
	int consumed = 0;
	try
	{
		--argc;
		++argv;
		consumed = cmd_args.Consume(argc, argv);
	}
	catch (const CmdArgs::Exception& error)
	{
		cerr << "Error: " << error.GetError() << endl;
		wtf = true;
	}

	if (wtf)
	{
		cout << "Reflang tool to generate reflection metadata." << endl;
		cout << "Usage:" << endl;
		cmd_args.PrintHelp();
		exit(-1);
	}

	int clang_argc = argc - consumed;
	char** clang_argv = &argv[consumed];

	parser::Options options;
	options.include = filter_include->Get();
	options.exclude = filter_exclude->Get();

	if (list_only->Get())
	{
		auto names = parser::GetSupportedTypeNames(
				clang_argc, clang_argv, options);
		for (const auto& it : names)
		{
			cout << it << endl;
		}
	}
	else
	{
		auto types = parser::GetTypes(clang_argc, clang_argv, options);
		serializer::Serialize(types);
	}
}
