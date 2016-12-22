#include "static-fields.src.hpp"
#include "static-fields.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

int MyClass::var = 456;
const int MyClass::const_var = MyClass::var;
int* MyClass::pointer_var = &MyClass::var;
int const* MyClass::const_pointer_var = &MyClass::var;
int const* const MyClass::const_pointer_const_var = &MyClass::var;
int& MyClass::ref_var = MyClass::var;
int const& MyClass::const_ref_var = MyClass::var;

Test::ComplexType MyClass::complex{ 789 };
const Test::ComplexType MyClass::const_complex = MyClass::complex;
Test::ComplexType* MyClass::pointer_complex = &MyClass::complex;
Test::ComplexType const* MyClass::const_pointer_complex = &MyClass::complex;
Test::ComplexType const* const MyClass::const_pointer_const_complex = &MyClass::complex;
Test::ComplexType& MyClass::ref_complex = MyClass::complex;
Test::ComplexType const& MyClass::const_ref_complex = MyClass::complex;

void RequireEquals(int i)
{
	REQUIRE(MyClass::var == i);
}

void RequireEquals(int const* const i)
{
	REQUIRE(&MyClass::var == i);
}

void RequireEquals(Test::ComplexType t)
{
	REQUIRE(MyClass::complex.field == t.field);
}

void RequireEquals(Test::ComplexType const* const t)
{
	REQUIRE(&MyClass::complex == t);
}

TEST_CASE("iterate-static-fields")
{
	int fields = Class<MyClass>::StaticFieldCount;
	REQUIRE(fields == 14);

	// non-const iteration.
	Class<MyClass>::IterateStaticFields(
			[&fields](auto& m)
			{
				RequireEquals(m);
				--fields;
			});

	REQUIRE(fields == 0);
}
