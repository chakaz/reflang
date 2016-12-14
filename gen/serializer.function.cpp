#include "serializer.function.hpp"
using namespace std;
using namespace reflang;

void serializer::SerializeFunction(ostream& o, const Function& f)
{
	string name_without_colons = f.GetFullName();
	std::replace(
			name_without_colons.begin(), name_without_colons.end(), ':', '_');

	o << R"(template <>
class Function<decltype()" << f.GetFullName() << "), " << f.GetFullName()
		<< R"(> : public IFunction
{
	int num_args() const override
	{
		return )" << f.Arguments.size() << R"(;
	}

	const std::string& GetName() override
	{
		static std::string name = ")" << f.GetFullName() << R"(";
		return name;
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

namespace
{
	// Object to auto-register )" << f.GetFullName() << R"(.
	struct )" << name_without_colons << R"(_registrar
	{
		)" << name_without_colons << R"(_registrar()
		{
			::reflang::registry::internal::Register(
				std::make_unique<
					Function<
						decltype()" << f.GetFullName() << R"(),
						)" << f.GetFullName() << R"(>>());
		}
	} )" << name_without_colons << R"(_instance;
}
)";
}
