#include "methods.src.hpp"
#include "methods.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

// MyClass method implementations
int global_int = 0;
void MyClass::Method0() { ++global_int; }
void MyClass::Method1(bool b, int i)
{
	if (b)
	{
		global_int = i;
	}
}
bool MyClass::RMethod0() { ++global_int; return true; }
bool MyClass::RMethod1(bool b, int i) { global_int = i; return b; }
void MyClass::VirtualMethod() { ++global_int; }
void MyClass::ConstMethod() const { ++global_int; }
void MyClass::MethodWithClassArg(ComplexArgument arg) { global_int = arg.i; }
void MyClass::MethodWithPointerArg(int* p) { global_int = *p; }
void MyClass::MethodWithConstReferenceArg0(const int& i) { global_int = i; }
void MyClass::MethodWithConstReferenceArg1(const ComplexArgument& arg) { global_int = arg.i; }

TEST_CASE("simple-methods")
{
	MyClass c;

	Method<decltype(&MyClass::Method0), &MyClass::Method0> m0;
	global_int = 0;
	Object result = m0(c);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 1);

	Method<decltype(&MyClass::Method1), &MyClass::Method1> m1;
	global_int = 0;
	result = m1(c, false, 100);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 0);
	result = m1(c, true, 100);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 100);
}

TEST_CASE("return-value-methods")
{
	MyClass c;

	Method<decltype(&MyClass::RMethod0), &MyClass::RMethod0> rm0;
	global_int = 0;
	Object result = rm0(c);
	REQUIRE(result.GetT<bool>() == true);
	REQUIRE(global_int == 1);

	Method<decltype(&MyClass::RMethod1), &MyClass::RMethod1> rm1;
	global_int = 0;
	result = rm1(c, true, 100);
	REQUIRE(result.GetT<bool>() == true);
	REQUIRE(global_int == 100);
	result = rm1(c, false, 200);
	REQUIRE(result.GetT<bool>() == false);
	REQUIRE(global_int == 200);
}

TEST_CASE("virtual-method")
{
	MyClass c;

	Method<decltype(&MyClass::VirtualMethod), &MyClass::VirtualMethod> vm;
	global_int = 0;
	Object result = vm(c);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 1);
}

TEST_CASE("const-method")
{
	MyClass c;

	Method<decltype(&MyClass::ConstMethod), &MyClass::ConstMethod> cm;
	global_int = 0;
	Object result = cm(c);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 1);
}

TEST_CASE("class-argument")
{
	MyClass c;

	Method<decltype(&MyClass::MethodWithClassArg), &MyClass::MethodWithClassArg> carg;
	global_int = 0;
	Object result = carg(c, ComplexArgument{ 123 });
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 123);
}

TEST_CASE("pointer-argument")
{
	MyClass c;

	Method<decltype(&MyClass::MethodWithPointerArg), &MyClass::MethodWithPointerArg> pm;
	global_int = 0;
	int i = 123;
	Object result = pm(c, &i);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 123);
}

TEST_CASE("const-ref-argument")
{
	MyClass c;

	Method<decltype(&MyClass::MethodWithConstReferenceArg0), &MyClass::MethodWithConstReferenceArg0> cr0;
	global_int = 0;
	Object result = cr0(c, 123);
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 123);

	Method<decltype(&MyClass::MethodWithConstReferenceArg1), &MyClass::MethodWithConstReferenceArg1> cr1;
	global_int = 0;
	result = cr1(c, ComplexArgument{ 456 });
	REQUIRE(result.IsVoid());
	REQUIRE(global_int == 456);
}

TEST_CASE("get-method")
{
	MyClass c;
	Class<MyClass> metadata;

	REQUIRE(metadata.GetMethod("non-existing").empty());

	auto methods = metadata.GetMethod("Method0");
	REQUIRE(methods.size() == 1);
	global_int = 0;
	auto& method = *(methods[0].get());
	method(c);
	REQUIRE(global_int == 1);
}
