#include "c-enum.src.hpp"
#include "c-enum.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("iterate")
{
	int i = 0;
	for (const auto& it : Enum<CEnum>::Iterate())
	{
		switch (i)
		{
			case 0:
				REQUIRE(it == CEnum::Value0);
				break;
			case 1:
				REQUIRE(it == CEnum::Value1);
				break;
			default:
				REQUIRE(false);
				break;
		}
		++i;
	}
}

TEST_CASE("to string")
{
	REQUIRE(Enum<CEnum>::Translate(CEnum::Value0) == "Value0");
	REQUIRE(Enum<CEnum>::Translate(CEnum::Value1) == "Value1");
}

TEST_CASE("from string")
{
	auto FromString = [](const string& s)
	{
		CEnum e;
		REQUIRE(Enum<CEnum>::TryTranslate(s, e));
		return e;
	};
	REQUIRE(FromString("Value0") == CEnum::Value0);
	REQUIRE(FromString("Value1") == CEnum::Value1);

	CEnum e;
	REQUIRE(!Enum<CEnum>::TryTranslate("UnknownString", e));
}
