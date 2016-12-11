#include "void-noargs.src.hpp"
#include "void-noargs.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("global")
{
	global_string = nullptr;
	std::unique_ptr<IFunction> func =
		std::make_unique<Function<decltype(GlobalFunction), GlobalFunction>>();
	(*func)({});
	REQUIRE(global_string == "GlobalFunction()");
}

TEST_CASE("namespace")
{
	global_string = nullptr;
	std::unique_ptr<IFunction> func = std::make_unique<Function<
		decltype(ns::NamespacedFunction),
		ns::NamespacedFunction>>();
	(*func)({});
	REQUIRE(global_string == "NamespacedFunction()");
}
