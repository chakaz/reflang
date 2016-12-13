class DummyClass {};

namespace ns
{
	const char* NamespacedFunction()
	{
		return "NamespacedFunction()";
	}
}

const char* GlobalFunction()
{
	return "GlobalFunction()";
}

DummyClass ReturnByValue()
{
	return DummyClass();
}

const DummyClass& ReturnByReference()
{
	static DummyClass dc;
	return dc;
}
