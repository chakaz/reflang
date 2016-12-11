namespace Test
{
	class ComplexArgument {};
	using MyInt = int;
}

class MyClass {
public:
	using ComplexArgument = Test::ComplexArgument;

	MyClass() = default;
	virtual ~MyClass() = default;

	void Method0() {}
	void Method1(bool b) {}
	void Method2(bool b, int i) {}
	void Method3(bool b, int i, float f) {}
	bool RMethod3(bool b, int i, float f) { return true; }
	bool RMethod1(bool) { return true; }
	virtual void VirtualMethod() {}
	virtual void AbstractMethod() = 0;
	void ConstMethod() const {}

	void MethodWithClassArg(ComplexArgument arg) {}
	void MethodWithPointerArg1(int* arg) {}
	void MethodWithPointerArg2(ComplexArgument* arg) {}
	void MethodWithReferenceArg1(int& arg) {}
	void MethodWithReferenceArg2(ComplexArgument& arg) {}
	void MethodWithConstReferenceArg1(const int& arg) {}
	void MethodWithConstReferenceArg2(const ComplexArgument& arg) {}

	void MethodWithNullptrt(nullptr_t arg) {}

	void MethodWithTypedef(MyInt arg) {}

	template <typename T>
	void TemplatedMethod() {}

protected:
	void ProtectedMethod() {}

private:
	void PrivateMethod() {}
};
