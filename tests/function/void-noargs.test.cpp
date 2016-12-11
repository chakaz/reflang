#include "void-noargs.src.hpp"
#include "void-noargs.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("simple")
{
	REQUIRE(true);
}
