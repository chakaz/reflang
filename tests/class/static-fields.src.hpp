#ifndef STATIC_FIELDS_SRC_HPP
#define STATIC_FIELDS_SRC_HPP

namespace Test
{
	class ComplexType
	{
	public:
		int field;
	};
}

class MyClass {
public:
	using ComplexType = Test::ComplexType;

	static int var;
	static const int const_var;
	static int* pointer_var;
	static int const* const_pointer_var;
	static int const* const const_pointer_const_var;
	static int& ref_var;
	static int const& const_ref_var;

	static ComplexType complex;
	static const ComplexType const_complex;
	static ComplexType* pointer_complex;
	static ComplexType const* const_pointer_complex;
	static ComplexType const* const const_pointer_const_complex;
	static ComplexType& ref_complex;
	static ComplexType const& const_ref_complex;

protected:
	static int protected_static;

private:
	static int private_static;
};

#endif //STATIC_FIELDS_SRC_HPP
