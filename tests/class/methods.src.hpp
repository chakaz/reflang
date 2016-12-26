#ifndef METHODS_SRC_HPP
#define METHODS_SRC_HPP

struct ComplexArgument { int i; };

class MyClass {
public:
	MyClass() = default;
	virtual ~MyClass() = default;

	void Method0();
	void Method1(bool b, int i);
	bool RMethod0();
	bool RMethod1(bool b, int i);
	virtual void VirtualMethod();
	void ConstMethod() const;

	void MethodWithClassArg(ComplexArgument arg);
	void MethodWithPointerArg(int* p);
	void MethodWithConstReferenceArg0(const int& i);
	void MethodWithConstReferenceArg1(const ComplexArgument& arg);

	template <typename T> void TemplatedMethod() {}

protected:
	void ProtectedMethod();

private:
	void PrivateMethod();
};

#endif //METHODS_SRC_HPP
