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
	REQUIRE(result.IsT<const char*>());
	REQUIRE(!result.IsT<int>());
	REQUIRE(result.GetT<const char*>() == string("GlobalFunction()"));
}

TEST_CASE("namespace")
{
	auto functions = registry::GetFunctionByName("ns::NamespacedFunction");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	auto result = (*func)();
	REQUIRE(result.IsT<const char*>());
	REQUIRE(!result.IsT<int>());
	REQUIRE(result.GetT<const char*>() == string("NamespacedFunction()"));
}

TEST_CASE("with-classes")
{
	auto functions = registry::GetFunctionByName("ReturnByValue");
	REQUIRE(functions.size() == 1);
	auto func = functions[0];
	auto result = (*func)();
	REQUIRE(result.IsT<DummyClass>());

	functions = registry::GetFunctionByName("ReturnByReference");
	REQUIRE(functions.size() == 1);
	func = functions[0];
	result = (*func)();
	REQUIRE(result.IsT<DummyClass>());
}
