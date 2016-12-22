#include "members.src.hpp"
#include "members.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

void RequireEquals(const MyClass& c, int i)
{
	REQUIRE(c.member == i);
}

void RequireEquals(const MyClass& c, int const* const i)
{
	REQUIRE(c.member == *i);
}

void RequireEquals(const MyClass& c, const Test::ComplexType& i)
{
	REQUIRE(c.complex.member == i.member);
}

void RequireEquals(const MyClass& c, Test::ComplexType const* const i)
{
	REQUIRE(c.complex.member == i->member);
}

TEST_CASE("iterate-members")
{
	int members = Class<MyClass>::MemberCount;
	REQUIRE(members == 14);

	MyClass instance(123);

	// non-const iteration.
	Class<MyClass>::IterateMembers(
			instance,
			[&members, &instance](auto& m)
			{
				RequireEquals(instance, m);
				--members;
			});

	REQUIRE(members == 0);

	// const iteration.
	const MyClass& const_instance = instance;
	Class<MyClass>::IterateMembers(
			const_instance,
			[&members, &const_instance](auto& m)
			{
				RequireEquals(const_instance, m);
				++members;
			});

	REQUIRE(members == Class<MyClass>::MemberCount);
}
