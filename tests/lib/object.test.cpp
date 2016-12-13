#include "lib/reflang.hpp"

#include <string>

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

TEST_CASE("void")
{
	Object object;
	REQUIRE(object.is_void());
	REQUIRE(object.is_t<void>());
}

TEST_CASE("get-set-integral")
{
	Object object(4);
	REQUIRE(!object.is_void());
	REQUIRE(object.is_t<int>());
	REQUIRE(object.get_t<int>() == 4);
}

TEST_CASE("get-set-class")
{
	// copy.
	string s = "hello!";
	Object object(s);
	REQUIRE(object.is_t<string>());
	REQUIRE(object.get_t<string>() == s);

	// reference.
	const string& rs = "hello!";
	object = Object(rs);
	REQUIRE(object.is_t<string>());
	REQUIRE(object.get_t<string>() == s);

	// r-value reference.
	string rrs = s;
	object = Object(std::move(rrs));
	REQUIRE(object.is_t<string>());
	REQUIRE(object.get_t<string>() == s);
}

namespace
{
	struct LifetimeWatcher
	{
		static int constructions;
		static int destructions;

		LifetimeWatcher() { ++constructions; }
		~LifetimeWatcher() { ++destructions; }
	};

	int LifetimeWatcher::constructions = 0;
	int LifetimeWatcher::destructions = 0;
}

TEST_CASE("safe-destruction")
{
	{
		LifetimeWatcher::constructions = 0;
		Object object(LifetimeWatcher{});
		// We test for > 0 rather than == N because there might be RVO or
		// construction and then move.
		REQUIRE(LifetimeWatcher::constructions > 0);

		LifetimeWatcher::constructions = 0;
		LifetimeWatcher::destructions = 0;

		{
			Object object2 = std::move(object);
			REQUIRE(LifetimeWatcher::constructions == 0);
			REQUIRE(LifetimeWatcher::destructions == 0);
		}
		REQUIRE(LifetimeWatcher::constructions == 0);
		REQUIRE(LifetimeWatcher::destructions == 1);
	}
	REQUIRE(LifetimeWatcher::constructions == 0);
	REQUIRE(LifetimeWatcher::destructions == 1);
}
