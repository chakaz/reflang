#include "serializer.function.hpp"

#include <sstream>

#include "serializer.util.hpp"

using namespace std;
using namespace reflang;

namespace
{
	string CallFunction(const Function& f)
	{
		stringstream s;
		s << f.GetFullName() << "(";
		int i = 0;
		for (const auto& arg : f.Arguments)
		{
			s << "args[" << i << "].GetT<std::decay_t<" << arg.Type << ">>()";
			if (i != f.Arguments.size() - 1)
			{
				s << ", ";
			}
			++i;
		}
		s << ")";
		return s.str();
	}
}

void serializer::SerializeFunction(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(template <>
class Function<decltype(%name%), %name%> : public IFunction
{
	int GetParameterCount() const override
	{
		return %arg_count%;
	}

	const std::string& GetName() const override
	{
		static const std::string name = "%name%";
		return name;
	}

	Object Invoke(const std::vector<Object>& args) override
	{
		if (args.size() != %arg_count%)
		{
			throw std::invalid_argument("count");
		}
)";
	int i = 0;
	for (const auto& arg : f.Arguments)
	{
		tmpl << "		if (!args[" << i << "].IsT<std::decay_t<" << arg.Type << R"(>>())
		{
			throw std::invalid_argument(")" << arg.Name << R"(");
		}
)";
		++i;
	}

	if (f.ReturnType == "void")
	{
		tmpl << R"(
		%call_function%;
		return Object();)";
	}
	else
	{
		tmpl << R"(
		return Object(%call_function%);)";
	}
	tmpl << R"(
	}
};

namespace
{
	// Object to auto-register %name%.
	struct %name_without_colons%_registrar
	{
		%name_without_colons%_registrar()
		{
			::reflang::registry::internal::Register(
				std::make_unique<
					Function<
						decltype(%name%),
						%name%>>());
		}
	} %name_without_colons%_instance;
}
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
				{"%arg_count%", to_string(f.Arguments.size())},
				{"%call_function%", CallFunction(f)},
				{"%name_without_colons%", GetNameWithoutColons(f.GetFullName())}
			});
}
