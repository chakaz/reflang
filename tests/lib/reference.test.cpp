#include "lib/reflang.hpp"

#include <string>

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("get-set-integral")
{
	int i = 4;
	Reference ref(i);
	REQUIRE(ref.IsT<int>());
	REQUIRE(ref.GetT<int>() == 4);
	ref.GetT<int>() = 3;
	REQUIRE(i == 3);
}

TEST_CASE("get-set-const")
{
	const int i = 4;
	Reference ref(i);
	REQUIRE(!ref.IsT<int>());
	REQUIRE(ref.IsT<const int>());
	REQUIRE(ref.GetT<const int>() == 4);
}
