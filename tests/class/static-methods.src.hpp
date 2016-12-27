#ifndef STATIC_METHODS_SRC_HPP
#define STATIC_METHODS_SRC_HPP

struct ComplexArgument { int i; };

class MyClass {
public:
	static void Method0();
	static void Method1(int i);
	static bool RMethod0();
	static bool RMethod1(bool b, int i);
};

#endif //STATIC_METHODS_SRC_HPP
