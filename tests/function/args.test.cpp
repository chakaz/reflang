#include "args.src.hpp"
#include "args.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("void-return-type")
{
	global_int = 0;
	auto functions = registry::GetFunctionByName("Func");
	REQUIRE(functions.size() == 1);
	auto f = functions[0];
	(*f)(1234);
	REQUIRE(global_int == 1234);
}

TEST_CASE("multiple-args-bool-return-type")
{
	global_int = 0;
	global_float = 0.0f;
	auto functions = registry::GetFunctionByName("Func2");
	REQUIRE(functions.size() == 1);
	auto f = functions[0];
	Object ret = (*f)(1234, 5678.0f);
	REQUIRE(global_int == 1234);
	REQUIRE(global_float == 5678.0f);
	REQUIRE(ret.is_t<bool>());
	REQUIRE(ret.get_t<bool>() == true);
}
