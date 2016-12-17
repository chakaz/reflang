int global_int = 0;
float global_float = 0.0f;

void Func(int a)
{
	global_int = a;
}

bool Func2(int a, float b)
{
	global_int = a;
	global_float = b;
	return true;
}
