#include "c-enum.src.hpp"
#include "c-enum.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("iterate")
{
	for (const auto& it : Enum<CEnum>::Iterate())
	{
		REQUIRE(false);
	}
}

TEST_CASE("to string")
{
	REQUIRE(Enum<CEnum>::Translate(CEnum()) == "");
}

TEST_CASE("from string")
{
	CEnum e1;
	REQUIRE(!Enum<CEnum>::TryTranslate("", e1));
	REQUIRE(!Enum<CEnum>::TryTranslate("Hi", e1));
}
