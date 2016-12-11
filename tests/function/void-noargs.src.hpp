const char* global_string = nullptr;

namespace ns
{
	void NamespacedFunction()
	{
		global_string = "NamespacedFunction()";
	}
}

void GlobalFunction()
{
	global_string = "GlobalFunction()";
}
