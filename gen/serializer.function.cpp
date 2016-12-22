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

void serializer::SerializeFunctionHeader(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(
template <>
class Function<decltype(%name%), %name%> : public IFunction
{
	int GetParameterCount() const override;

	const std::string& GetName() const override;

	Object Invoke(const std::vector<Object>& args) override;
};
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
			});
}

void serializer::SerializeFunctionSources(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(
int Function<decltype(%name%), %name%>::GetParameterCount() const
{
	return %arg_count%;
}

static const std::string %escaped_name%_name = "%name%";

const std::string& Function<decltype(%name%), %name%>::GetName() const
{
	return %escaped_name%_name;
}

Object Function<decltype(%name%), %name%>::Invoke(const std::vector<Object>& args)
{
	if (args.size() != %arg_count%)
	{
		throw std::invalid_argument("count");
	}
)";
	int i = 0;
	for (const auto& arg : f.Arguments)
	{
		tmpl << "	if (!args[" << i << "].IsT<std::decay_t<" << arg.Type << R"(>>())
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

namespace
{
	// Object to auto-register %name%.
	struct %escaped_name%_registrar
	{
		%escaped_name%_registrar()
		{
			::reflang::registry::internal::Register(
				std::make_unique<
					Function<
						decltype(%name%),
						%name%>>());
		}
	} %escaped_name%_instance;
}
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
				{"%arg_count%", to_string(f.Arguments.size())},
				{"%call_function%", CallFunction(f)},
				{"%escaped_name%", GetNameWithoutColons(f.GetFullName())}
			});
}
