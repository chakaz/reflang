#include "cpp11-enum.src.hpp"
#include "cpp11-enum.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

using TestEnum = MyNamespace::MyClass::Cpp11Enum;

TEST_CASE("iterate")
{
	int i = 0;
	for (const auto& it : Enum<TestEnum>::Iterate())
	{
		switch (i)
		{
			case 0:
				REQUIRE(it == TestEnum::Value1);
				break;
			case 1:
				REQUIRE(it == TestEnum::Value2);
				break;
			case 2:
				REQUIRE(it == TestEnum::Value3);
				break;
			case 3:
				REQUIRE(it == TestEnum::Value5);
				break;
			case 4:
				REQUIRE(it == TestEnum::Value6);
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
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value1) == "Value1");
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value2) == "Value2");
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value3) == "Value3");
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value4) == "Value3");
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value5) == "Value5");
	REQUIRE(Enum<TestEnum>::Translate(TestEnum::Value6) == "Value6");
}

TEST_CASE("from string")
{
	auto FromString = [](const string& s)
	{
		TestEnum e;
		REQUIRE(Enum<TestEnum>::TryTranslate(s, e));
		return e;
	};
	REQUIRE(FromString("Value1") == TestEnum::Value1);
	REQUIRE(FromString("Value2") == TestEnum::Value2);
	REQUIRE(FromString("Value3") == TestEnum::Value3);
	REQUIRE(FromString("Value4") == TestEnum::Value4);
	REQUIRE(FromString("Value5") == TestEnum::Value5);
	REQUIRE(FromString("Value6") == TestEnum::Value6);

	TestEnum e;
	REQUIRE(!Enum<TestEnum>::TryTranslate("UnknownString", e));
}
