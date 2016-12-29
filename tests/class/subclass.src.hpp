#ifndef SUBCLASS_SRC_HPP
#define SUBCLASS_SRC_HPP

class MyClass {
public:

	class Subclass
	{
	public:
		int field = 0;
		static int static_field;
		void method();
		static void static_method();
	};
};

#endif //SUBCLASS_SRC_HPP
