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

## More details

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
Suppose you have the following `class`:

```cpp
	class MyClass
	{
	public:
		int field = 0;
		static int static_field;
		void method();
		static void static_method();
	};
```

With Reflang you could do:

```cpp
	reflang::Class<MyClass> metadata;
	MyClass c;

	// Modify / use c's 'field'.
	reflang::Reference ref = metadata.GetField(c, "field");
	ref.GetT<int>() = 10;

	// Modify / use 'static_field'.
	ref = metadata.GetStaticField("static_field")
	ref.GetT<int>() = 10;

	// Execute 'method()'.
	auto methods = metadata.GetMethod("method");
	(*methods[0])(c);

	// Execute 'static_method()'.
	auto methods = metadata.GetStaticMethod("static_method");
	(*methods[0])();
```

Or even without `#include`ing `MyClass`:

```cpp
	// Note: no need to include header of MyClass.
	IClass* metadata = reflang::registry::GetClassByName("MyClass");

	// Assign 10 to MyClass::static_field.
	metadata->GetStaticField("static_field").GetT<int>() = 10;

	// Invoke MyClass::static_method().
	auto static_methods = metadata->GetStaticMethod("static_method");
	(*static_methods[0])();

	reflang::Reference c = // get Reference wrapping a MyClass from somewhere.

	// Assign 10 to c.field.
	metadata->GetField(c, "field").GetT<int>() = 10;

	// Invoke c.method().
	auto methods = metadata->GetMethod("method");
	(*methods[0])(c);
```

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
	// string_value == "Value2".

	MyEnum e
	if (reflang::Enum<MyEnum>::TryTranslate(string_value, e))
	{
		// e == MyEnum::Value2.
	}
```

Or even without `#include`ing `MyEnum`:

```cpp
	// Note: no need to include header of MyEnum.
	IEnum* e = reglang::registry::GetEnumByName("MyEnum");

	// Iterate over {"Value1", "Value2", "Value3", "Value5", "Value6"}.
	for (const auto& s : e->GetStringValues())
	{
		// ...
	}

	// Iterate over {4, 5, 6, 0, 12}.
	for (const auto& s : e->GetStringValues())
	{
		// ...
	}
```

## How it works
Reflang is made of 2 components:
* **Code generator**, which uses libclang to parse your C++ code and generate
  reflection information. You can easily integrate this tool in your build
  process;
* **Support library**, which allows generic and abstract coding ("find class by
  name", "invoke function by name", etc).

Reflang understands your code exactly like clang does, so which results in
accurate parsing of your code.

## End-to-end Examples
Check out the [tests/](tests/) directory, which shows pretty much all features
supported by Reflang.

Each test has 4 files:
* `X.src.hpp` -- source code which will be used for reflection;
* `X.gen.hpp` -- generated reflection code declarations for `X.src.hpp`;
* `X.gen.cpp` -- generated reflection code definitions for `X.src.hpp`;
* `X.test.cpp` -- test code which tests the above.

## Using
*This section is incomplete. Please check later for more elaborated
documentation.*

Rough instructions:
* Clone the repository;
* Build it using CMake;
* A new executable, *reflang* is now available - use it to generate code;
* Link your program with the generated code and with *libreflang*, which has
  been built by CMake as well;
* Profit.

Examples of running the tool:

```bash
# print help
$ ./reflang

# generate code for test.hpp, write both hpp and cpp to stdout
$ ./reflang test.hpp

# specify output files
$ ./reflang --out-cpp test.reflang.cpp --out-hpp test.reflang.hpp test.hpp

# only list classes, functions, etc available for generation
$ ./reflang --list-only true test.hpp

# only generate code for classes, functions, etc not in std::
$ ./reflang --exclude "std::.*" test.hpp

# only generate code for classes, functions, etc beginning with My
$ ./reflang --include "My.*" test.hpp

# pass flags supported by libclang
$ ./reflang test.hpp -- -std=c++14 -DSOMETHING -I./include/
```

## Supported Features & Limitations
&nbsp;	| Feature
--------|-----------------------------------------------
✓		| *Supported*: Invoking global / namespace functions
✓		| *Supported*: Function overloads
✓		| *Supported*: Iterating classes' fields
✓		| *Supported*: Iterating classes' static fields
✓		| *Supported*: Iterating classes' methods
✓		| *Supported*: Iterating classes' static methods
✓		| *Supported*: Getting a class field / method / statics by name
✓		| *Supported*: Iterating enum values
✓		| *Supported*: Get vector of enum values
✓		| *Supported*: Get vector of enum string names
✓		| *Supported*: Converting enum <-> string
✓		| *Supported*: Use `class` / `function` / `enum` without `#include` (through registry)
⌛		| *Soon*: Get function's / method's argument names & types
⌛		| *Soon*: Get function's / method's return type
⌛		| *Soon*: Overloaded class methods
⌛		| *Soon*: Construct classes through reflection
⌛		| *Soon*: Find types by regex
⌛		| *Soon*: Respect functions / methods default values
⌛		| *Soon*: Support for operators
✕		| *Not supported*: `template` classes / functions
✕		| *Not supported*: Functions taking non-const reference arguments (yet?)
✕		| *Not supported*: Functions taking rvalue references arguments
✕		| *Not supported*: Private methods / fields
✕		| *Not supported*: Anything in anonymous namespace

## Bugs, pull requests, feedback
Please check this section later.

## Kind Words & Donations
Please check this section later.
