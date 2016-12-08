namespace MyNamespace
{

	class MyClass
	{
	public:

		enum class MyEnum
		{
			Value1 = 4,
			Value2,
			Value3,
			Value4 = Value3,
			Value5 = 0,
			Value6 = 12
		};

		enum class MyEnum2
		{
		};
	};

}
