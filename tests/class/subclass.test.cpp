#include "subclass.src.hpp"
#include "subclass.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

// subclass.src.hpp implementation
int MyClass::Subclass::static_field = 0;
void MyClass::Subclass::method() { ++field; }
void MyClass::Subclass::static_method() { ++static_field; }

TEST_CASE("field")
{
	Class<MyClass::Subclass> metadata;
	MyClass::Subclass c;
	metadata.GetField(c, "field").GetT<int>() = 10;
	REQUIRE(c.field == 10);
}

TEST_CASE("static-field")
{
	Class<MyClass::Subclass> metadata;

	MyClass::Subclass::static_field = 0;
	metadata.GetStaticField("static_field").GetT<int>() = 10;
	REQUIRE(MyClass::Subclass::static_field == 10);
}

TEST_CASE("method")
{
	Class<MyClass::Subclass> metadata;
	MyClass::Subclass c;
	auto methods = metadata.GetMethod("method");
	REQUIRE(methods.size() == 1);
	(*methods[0])(c);
	REQUIRE(c.field == 1);
}

TEST_CASE("static-method")
{
	Class<MyClass::Subclass> metadata;

	auto methods = metadata.GetStaticMethod("static_method");
	REQUIRE(methods.size() == 1);

	MyClass::Subclass::static_field = 0;
	(*methods[0])();
	REQUIRE(MyClass::Subclass::static_field == 1);
}

TEST_CASE("registry")
{
	IClass* metadata = registry::GetClassByName("MyClass::Subclass");
	REQUIRE(metadata != nullptr);

	MyClass::Subclass c;

	metadata->GetField(c, "field").GetT<int>() = 10;
	REQUIRE(c.field == 10);

	MyClass::Subclass::static_field = 0;
	metadata->GetStaticField("static_field").GetT<int>() = 10;
	REQUIRE(MyClass::Subclass::static_field == 10);

	auto methods = metadata->GetMethod("method");
	REQUIRE(methods.size() == 1);
	(*methods[0])(c);
	REQUIRE(c.field == 11);

	auto static_methods = metadata->GetStaticMethod("static_method");
	REQUIRE(static_methods.size() == 1);
	(*static_methods[0])();
	REQUIRE(MyClass::Subclass::static_field == 11);
}
