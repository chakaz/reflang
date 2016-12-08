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
