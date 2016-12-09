#include "empty-enum.src.hpp"
#include "empty-enum.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("iterate")
{
	for (const auto& it : Enum<EmptyEnum>::Iterate())
	{
		REQUIRE(false);
	}

	for (const auto& it : Enum<EmptyCEnum>::Iterate())
	{
		REQUIRE(false);
	}
}

TEST_CASE("to string")
{
	REQUIRE(Enum<EmptyEnum>::Translate(EmptyEnum()) == "");
	REQUIRE(Enum<EmptyCEnum>::Translate(EmptyCEnum()) == "");
}

TEST_CASE("from string")
{
	EmptyEnum e1;
	REQUIRE(!Enum<EmptyEnum>::TryTranslate("", e1));
	REQUIRE(!Enum<EmptyEnum>::TryTranslate("Hi", e1));
	EmptyCEnum e2;
	REQUIRE(!Enum<EmptyCEnum>::TryTranslate("", e2));
	REQUIRE(!Enum<EmptyCEnum>::TryTranslate("Hi", e2));
}
