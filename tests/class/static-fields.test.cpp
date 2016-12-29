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
constexpr const int MyClass::constexpr_var;

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
	REQUIRE(fields == 15);

	// non-const iteration.
	Class<MyClass>::IterateStaticFields(
			[&fields](auto& m)
			{
				RequireEquals(m);
				--fields;
			});

	REQUIRE(fields == 0);
}

TEST_CASE("get-static-field")
{
	Class<MyClass> metadata;
	Reference field = metadata.GetStaticField("var");

	MyClass::var = 1;
	REQUIRE(field.GetT<int>() == 1);
	field.GetT<int>() = 2;
	REQUIRE(MyClass::var == 2);

	REQUIRE_THROWS(metadata.GetStaticField("non-existing"));
}
