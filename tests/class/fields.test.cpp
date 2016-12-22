#include "fields.src.hpp"
#include "fields.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

void RequireEquals(const MyClass& c, int i)
{
	REQUIRE(c.field == i);
}

void RequireEquals(const MyClass& c, int const* const i)
{
	REQUIRE(c.field == *i);
}

void RequireEquals(const MyClass& c, const Test::ComplexType& i)
{
	REQUIRE(c.complex.field == i.field);
}

void RequireEquals(const MyClass& c, Test::ComplexType const* const i)
{
	REQUIRE(c.complex.field == i->field);
}

TEST_CASE("iterate-fields")
{
	int fields = Class<MyClass>::FieldCount;
	REQUIRE(fields == 14);

	MyClass instance(123);

	// non-const iteration.
	Class<MyClass>::IterateFields(
			instance,
			[&fields, &instance](auto& m)
			{
				RequireEquals(instance, m);
				--fields;
			});

	REQUIRE(fields == 0);

	// const iteration.
	const MyClass& const_instance = instance;
	Class<MyClass>::IterateFields(
			const_instance,
			[&fields, &const_instance](auto& m)
			{
				RequireEquals(const_instance, m);
				++fields;
			});

	REQUIRE(fields == Class<MyClass>::FieldCount);
}
