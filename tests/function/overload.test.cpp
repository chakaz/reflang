#include "overload.src.hpp"
#include "overload.gen.hpp"

#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"

using namespace reflang;
using namespace std;

// implementation for overload.src.hpp
int global_int = 0;
float global_float = 0.0f;
void Func() { ++global_int; }
void Func(int i) { global_int = i; }
void Func(float f) { global_float = f; }
bool Func(const char& c) { return c == '!'; }

TEST_CASE("void() with registry")
{
	global_int = 0;
	auto functions = registry::GetFunctionByName("Func");
	REQUIRE(functions.size() == 4);

	// Find relevant overload.
	IFunction* f = nullptr;
	for (const auto& func : functions)
	{
		if (func->GetParameterCount() == 0)
		{
			f = func;
			break;
		}
	}
	REQUIRE(f != nullptr);

	(*f)();
	REQUIRE(global_int == 1);
}

TEST_CASE("void(int) with registry")
{
	global_int = 0;
	auto functions = registry::GetFunctionByName("Func");
	REQUIRE(functions.size() == 4);

	// Find relevant overload.
	IFunction* f = nullptr;
	for (const auto& func : functions)
	{
		if (func->GetParameterCount() == 1
			&& func->GetParameter(0).Type == "int")
		{
			f = func;
			break;
		}
	}
	REQUIRE(f != nullptr);

	(*f)(123);
	REQUIRE(global_int == 123);
}

TEST_CASE("void(float) with registry")
{
	global_float = 0.0f;
	auto functions = registry::GetFunctionByName("Func");
	REQUIRE(functions.size() == 4);

	// Find relevant overload.
	IFunction* f = nullptr;
	for (const auto& func : functions)
	{
		if (func->GetParameterCount() == 1
			&& func->GetParameter(0).Type == "float")
		{
			f = func;
			break;
		}
	}
	REQUIRE(f != nullptr);

	(*f)(123.0f);
	REQUIRE(global_float == 123.0f);
}

TEST_CASE("bool(char) with registry")
{
	auto functions = registry::GetFunctionByName("Func");
	REQUIRE(functions.size() == 4);

	// Find relevant overload.
	IFunction* f = nullptr;
	for (const auto& func : functions)
	{
		if (func->GetParameterCount() == 1
			&& func->GetParameter(0).Type == "const char &"
			&& func->GetReturnType().Type == "bool")
		{
			f = func;
			break;
		}
	}
	REQUIRE(f != nullptr);

	Object result = (*f)('h');
	REQUIRE(result.GetT<bool>() == false);
	result = (*f)('!');
	REQUIRE(result.GetT<bool>() == true);
}
