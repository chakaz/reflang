#include "return-noargs.src.hpp"
#include "return-noargs.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("global")
{
	std::unique_ptr<IFunction> func =
		std::make_unique<Function<decltype(GlobalFunction), GlobalFunction>>();
	auto result = (*func)({});
	REQUIRE(result.is_t<const char*>());
	REQUIRE(!result.is_t<int>());
	REQUIRE(result.get_t<const char*>() == "GlobalFunction()");
}

TEST_CASE("namespace")
{
	std::unique_ptr<IFunction> func = std::make_unique<Function<
		decltype(ns::NamespacedFunction),
		ns::NamespacedFunction>>();
	auto result = (*func)({});
	REQUIRE(result.is_t<const char*>());
	REQUIRE(!result.is_t<int>());
	REQUIRE(result.get_t<const char*>() == "NamespacedFunction()");
}
