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
	REQUIRE(i == 5);
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

TEST_CASE("reflection")
{
	auto enums = registry::GetEnumByName("MyNamespace::MyClass::Cpp11Enum");
	REQUIRE(enums.size() == 1);
	auto e = enums[0];
	REQUIRE(e->GetStringValues() ==
			vector<string>({"Value1", "Value2", "Value3", "Value5", "Value6"}));
	REQUIRE(e->GetIntValues() == vector<int>({4, 5, 6, 0, 12}));

	int i = 0;
	REQUIRE(!e->TryTranslate("????", i));
	REQUIRE(e->TryTranslate("Value1", i));
	REQUIRE(i == 4);
	REQUIRE(e->TryTranslate("Value2", i));
	REQUIRE(i == 5);
	REQUIRE(e->TryTranslate("Value3", i));
	REQUIRE(i == 6);
	REQUIRE(e->TryTranslate("Value4", i));
	REQUIRE(i == 6);
	REQUIRE(e->TryTranslate("Value5", i));
	REQUIRE(i == 0);
	REQUIRE(e->TryTranslate("Value6", i));
	REQUIRE(i == 12);

	string s;
	REQUIRE(!e->TryTranslate(1234, s));
	REQUIRE(e->TryTranslate(4, s));
	REQUIRE(s == "Value1");
	REQUIRE(e->TryTranslate(5, s));
	REQUIRE(s == "Value2");
	REQUIRE(e->TryTranslate(6, s));
	REQUIRE(s == "Value3");
	REQUIRE(e->TryTranslate(0, s));
	REQUIRE(s == "Value5");
	REQUIRE(e->TryTranslate(12, s));
	REQUIRE(s == "Value6");
}
