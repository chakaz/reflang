#include "void-noargs.src.hpp"
#include "void-noargs.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("global")
{
	global_string = nullptr;
	auto functions = registry::GetFunctionByName("GlobalFunction");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	Object result = (*func)();
	REQUIRE(result.is_void());
	REQUIRE(result.is_t<void>());
	REQUIRE(global_string == string("GlobalFunction()"));
}

TEST_CASE("namespace")
{
	global_string = nullptr;
	auto functions = registry::GetFunctionByName("ns::NamespacedFunction");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	Object result = (*func)();
	REQUIRE(result.is_void());
	REQUIRE(result.is_t<void>());
	REQUIRE(global_string == string("NamespacedFunction()"));
}

TEST_CASE("non-existent")
{
	auto functions = registry::GetFunctionByName("waaaaaaat");
	REQUIRE(functions.size() == 0);
}
