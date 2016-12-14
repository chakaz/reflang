#include "return-noargs.src.hpp"
#include "return-noargs.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("global")
{
	auto functions = registry::GetFunctionByName("GlobalFunction");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	auto result = (*func)();
	REQUIRE(result.is_t<const char*>());
	REQUIRE(!result.is_t<int>());
	REQUIRE(result.get_t<const char*>() == "GlobalFunction()");
}

TEST_CASE("namespace")
{
	auto functions = registry::GetFunctionByName("ns::NamespacedFunction");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	auto result = (*func)();
	REQUIRE(result.is_t<const char*>());
	REQUIRE(!result.is_t<int>());
	REQUIRE(result.get_t<const char*>() == "NamespacedFunction()");
}

TEST_CASE("with-classes")
{
	auto functions = registry::GetFunctionByName("ReturnByValue");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	auto result = (*func)();
	REQUIRE(result.is_t<DummyClass>());

	functions = registry::GetFunctionByName("ReturnByReference");
	REQUIRE(functions.size() == 1);
	func = functions[0];
	result = (*func)();
	REQUIRE(result.is_t<DummyClass>());
}
