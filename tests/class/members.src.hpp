namespace Test
{
	class ComplexType {};
}

class MyClass {
public:
	using ComplexType = Test::ComplexType;

	MyClass()
	:	member(0)
	,	const_member(member)
	,	pointer_member(&member)
	,	const_pointer_member(&member)
	,	const_pointer_const_member(&member)
	,	ref_member(member)
	,	const_ref_member(member)
	,	const_complex(complex)
	,	pointer_complex(&complex)
	,	const_pointer_complex(&complex)
	,	const_pointer_const_complex(&complex)
	,	ref_complex(complex)
	,	const_ref_complex(complex)
	{
	}

	int member;
	const int const_member;
	int* pointer_member;
	int const* const_pointer_member;
	int const* const const_pointer_const_member;
	int& ref_member;
	int const& const_ref_member;

	ComplexType complex;
	const ComplexType const_complex;
	ComplexType* pointer_complex;
	ComplexType const* const_pointer_complex;
	ComplexType const* const const_pointer_const_complex;
	ComplexType& ref_complex;
	ComplexType const& const_ref_complex;

protected:
	int protected_member;

private:
	int private_member;
};