#include "static-methods.src.hpp"
#include "static-methods.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

// implementation for static-methods.src.hpp
int global_int = 0;

void MyClass::Method0() { ++global_int; }

TEST_CASE("simple-methods")
{
}
