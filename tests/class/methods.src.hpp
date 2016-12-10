class MyClass {
public:
	MyClass() = default;
	virtual ~MyClass() = default;

	void Method() {}
	virtual void VirtualMethod() {}
	virtual void AbstractMethod() = 0;
	void ConstMethod() const {}

protected:
	void ProtectedMethod() {}

private:
	void PrivateMethod() {}
};
