#include "static-methods.src.hpp"
#include "static-methods.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

// implementation for static-methods.src.hpp
int global_int = 0;

void MyClass::Method0() { ++global_int; }
void MyClass::Method1(int i) { global_int = i; }
bool MyClass::RMethod0() { ++global_int; return true; }
bool MyClass::RMethod1(bool b, int i) {
	if (b)
	{
		global_int = i;
	}
	return b;
}

TEST_CASE("Method0")
{
	global_int = 0;
	Function<decltype(MyClass::Method0), MyClass::Method0> m0;
	m0();
	REQUIRE(global_int == 1);
}

TEST_CASE("Method1")
{
	global_int = 0;
	Function<decltype(MyClass::Method1), MyClass::Method1> m1;
	m1(123);
	REQUIRE(global_int == 123);
}

TEST_CASE("RMethod0")
{
	global_int = 0;
	Function<decltype(MyClass::RMethod0), MyClass::RMethod0> rm0;
	Object result = rm0();
	REQUIRE(global_int == 1);
	REQUIRE(result.GetT<bool>() == true);
}

TEST_CASE("RMethod1")
{
	global_int = 0;
	Function<decltype(MyClass::RMethod1), MyClass::RMethod1> rm1;
	Object result = rm1(false, 123);
	REQUIRE(global_int == 0);
	REQUIRE(result.GetT<bool>() == false);
	result = rm1(true, 123);
	REQUIRE(global_int == 123);
	REQUIRE(result.GetT<bool>() == true);
}

TEST_CASE("with-registry")
{
	auto functions = registry::GetFunctionByName("MyClass::RMethod1");
	REQUIRE(functions.size() == 1);
	auto f = functions[0];

	global_int = 0;
	Object result = (*f)(false, 123);
	REQUIRE(global_int == 0);
	REQUIRE(result.GetT<bool>() == false);
	result = (*f)(true, 123);
	REQUIRE(global_int == 123);
	REQUIRE(result.GetT<bool>() == true);
}
