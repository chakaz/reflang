# Reflang - Modern C++ reflection using libclang

## TL;DR
**Reflang is a reflection library & tool for modern C++.**

The tool parses your C++ code using libclang, then generates *human-readable*
code around it to enable reflection.

The library is extremely lightweight, containing utility interfaces, classes &
functions.

Reflang's library has no dependencies other than C++14 with STL. Reflang's
tool's only other dependency is libclang (which is not required after code
generation).

### Enums
Suppose you have the following `enum`:

```cpp
	enum class MyEnum
	{
		Value1 = 4,
		Value2,
		Value3,
		Value4 = Value3,
		Value5 = 0,
		Value6 = 12
	};
```

With Reflang you could do:

```cpp
	for (auto it : reflang::Enum<MyEnum>::Iterate())
	{
		// Do something with MyEnum values...
	}

	std::string string_value = reflang::Enum<MyEnum>::Translate(MyEnum::Value2);
	// string_value == "Value2"

	MyEnum e
	if (reflang::Enum<MyEnum>::TryTranslate(string_value, e))
	{
		// e == MyEnum::Value2
	}
```

Or even without `#include`ing `MyEnum`:

```cpp
	// Note: no need to include header of MyEnum
	IEnum* e = reglang::registry::GetEnumByName("MyEnum");

	// Iterate over {"Value1", "Value2", "Value3", "Value5", "Value6"}
	for (const auto& s : e->GetStringValues())
	{
		// ...
	}

	// Iterate over {4, 5, 6, 0, 12}
	for (const auto& s : e->GetStringValues())
	{
		// ...
	}
```

### Functions
Suppose you have the following function:

```cpp
	bool Func(int a, float b);
```

With Reflang you could do:

```cpp
	// Note: no need to include header of Func().
	std::vector<IFunction*> functions = reflang::registry::GetFunctionByName("Func");
	IFunction* f = functions[0];
	Object ret = (*f)(1234, 5678.0f);
	if (ret.GetT<bool>())
	{
		// ...
	}
```

### Classes
Reflang will soon support classes and other forms of fun.

## How it works
Reflang is made of 2 components:
* Code generator, which uses libclang to parse your C++ code and generate
  reflection information. You can easily integrate this tool in your build
  process;
* Support library, which allows generic and abstract coding ("find class by
  name", "invoke function by name", etc).

Reflang understands your code exactly like clang does, so which results in
accurate parsing of your code.

## Examples
Check out the tests/ directory, which shows pretty much all features supported
by Reflang.

Each test is made of 4 files:
* X.src.hpp -- source code which will be used for reflection;
* X.gen.hpp -- generated reflection code declarations (try reading it!);
* X.gen.cpp -- generated reflection code definitions (try reading it!);
* X.test.cpp -- test code which uses the above.

## Using
Please check this section later.

## Building
Please check this section later.

## Limitations
* No `template` classes / functions support.
* No support for functions taking non-const reference arguments (yet?).
* No support for functions taking rvalue references arguments.

## Bugs, pull requests, feedback
Please check this section later.

## Donations
Please check this section later.
