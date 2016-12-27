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
	auto reflang_include = cmd_args.Register<string>(
			"--reflang-include",
			"Complete #include line for reflang for generated code.",
			R"(#include "reflang.hpp")");
	auto out_hpp = cmd_args.Register<string>(
			"--out-hpp",
			"Output file path to write declarations (header) to. If empty "
			"stdout is used (outputs to console).",
			"");
	auto out_cpp = cmd_args.Register<string>(
			"--out-cpp",
			"Output file path to write definitions to. If empty, --out-hpp is "
			"used.",
			"");

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

	if (wtf || consumed == 0)
	{
		cout << "Reflang tool to generate reflection metadata.\n";
		cout << "\n";
		cout << "Usage: reflang [reflang_flags] -- [clang_flags]\n";
		cout << "Where [reflang_flags] are any of the below, and [clang_flags] "
			"are any flags supported by the libclang version installed\n";
		cout << "\n";
		cout << "Supported flags:\n";
		cmd_args.PrintHelp();
		exit(-1);
	}

	int clang_argc = argc - consumed;
	char** clang_argv = &argv[consumed];

	parser::Options options;
	options.include = "^(" + filter_include->Get() + ")$";
	options.exclude = "^(" + filter_exclude->Get() + ")$";

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
		serializer::Options options;
		options.include_path = reflang_include->Get();
		options.out_hpp_path = out_hpp->Get();
		options.out_cpp_path = out_cpp->Get();
		serializer::Serialize(types, options);
	}
}
