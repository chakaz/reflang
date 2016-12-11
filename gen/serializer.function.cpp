#include "serializer.function.hpp"
using namespace std;
using namespace reflang;

void serializer::SerializeFunction(ostream& o, const Function& f)
{
	o << R"(template <>
class Function<decltype()" << f.GetFullName() << "), " << f.GetFullName() << R"(> : public IFunction
{
	int num_args() const override
	{
		return )" << f.Arguments.size() << R"(;
	}

	Object Invoke(const std::vector<Object>& args) override
	{
		if (args.size() != this->num_args())
		{
			throw std::invalid_argument("count");
		}
)";
	if (f.ReturnType == "void")
	{
		o << "		" << f.GetFullName() << "();\n";
		o << "		return Object();\n";
	}
	else
	{
		o << "		return Object(" << f.GetFullName() << "());\n";
	}
	o << R"(	}
};
)";
}
