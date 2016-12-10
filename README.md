# Reflang - Modern C++ reflection using libclang

## In short
Reflang is a reflection library & tool for C++. For example, if your source
looks like this:

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

Using reflang you could do:

```cpp
	for (auto it : reflang::Enum<MyEnum>::Iterate())
	{
		// do something with MyEnum values
	}

	std::string string_value = reflang::Enum<MyEnum>::Translate(MyEnum::Value2);
	// string_value == "Value2"

	MyEnum e
	if (reflang::Enum<MyEnum>::TryTranslate(string_value, e))
	{
		// e == MyEnum::Value2
	}
```

Reflang will soon support classes, functions and other forms of fun.

## How it works
Reflang is made of 2 components:
* Code generator, which uses libclang to parse your C++ code and generate
  reflection information;
* Support library, which allows generic and abstract coding ("find class by
  name", "invoke method by name", etc).

Reflang understands your code exactly like clang does, so by combining reflang
in your build process you can get always updated reflection information with no
effort.

## Using
Please check this section later.

## Building
Please check this section later.

## Bugs, pull requests, feedback
Please check this section later.

## Donations
Please check this section later.
