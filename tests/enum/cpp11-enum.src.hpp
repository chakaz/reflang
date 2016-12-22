#ifndef CPP11_ENUM_SRC_HPP
#define CPP11_ENUM_SRC_HPP

namespace MyNamespace
{
	class MyClass
	{
	public:

		// C++11 enum with repetitions, not continuous, etc.
		enum class Cpp11Enum
		{
			Value1 = 4,
			Value2,
			Value3,
			Value4 = Value3,
			Value5 = 0,
			Value6 = 12
		};
	};
}

#endif //CPP11_ENUM_SRC_HPP
