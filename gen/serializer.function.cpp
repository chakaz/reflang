#include "serializer.function.hpp"

#include <sstream>
#include <unordered_map>

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

	// Returns a different string for equal names. This is useful for overload
	// disambiguation. For example, first time called with "foo" it will return
	// "", second time will return "_1", third time will return "_2", etc.
	string GetUniqueSuffixForString(const string& name)
	{
		static unordered_map<string, int> unique_ids;
		int current_id = unique_ids[name]++;
		string unique_id = "_" + to_string(current_id);
		if (unique_id == "_0")
		{
			// No need to make the generated code ugly for the first instance of
			// the function (which probably has no overloads anyway).
			unique_id.clear();
		}
		return unique_id;
	}
}

string serializer::GetFunctionSignature(const Function& f)
{
	stringstream s;
	s << f.ReturnType << "(*)(";
	for (size_t i = 0; i < f.Arguments.size(); ++i)
	{
		s << f.Arguments[i].Type;
		if (i != f.Arguments.size() - 1)
		{
			s << ", ";
		}
	}
	s << ")";
	return s.str();
}

void serializer::SerializeFunctionHeader(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(
template <>
class Function<%signature%, %name%> : public IFunction
{
	int GetParameterCount() const override;
	Parameter GetReturnType() const override;
	Parameter GetParameter(int i) const override;

	const std::string& GetName() const override;

	Object Invoke(const std::vector<Object>& args) override;
};
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
				{"%signature%", serializer::GetFunctionSignature(f)}
			});
}

void serializer::SerializeFunctionSources(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(
int Function<%signature%, %name%>::GetParameterCount() const
{
	return %arg_count%;
}

Parameter Function<%signature%, %name%>::GetReturnType() const
{
	Parameter result;
	result.Type = "%return_type%";
	return result;
}

Parameter Function<%signature%, %name%>::GetParameter(int i) const
{
	if (i < 0 || i >= GetParameterCount())
	{
		throw Exception("Argument out of range.");
	}

	Parameter result;
)";
	if (!f.Arguments.empty())
	{
		tmpl << R"(
	switch (i)
	{
)";
		for (size_t i = 0; i < f.Arguments.size(); ++i)
		{
			tmpl << "	case " << i << ":\n";
			tmpl << "		result.Name = \"" << f.Arguments[i].Name << "\";\n";
			tmpl << "		result.Type = \"" << f.Arguments[i].Type << "\";\n";
			tmpl << "		break;\n";
		}
		tmpl << R"(	default:
		break;
	}

)";
	}
	tmpl << R"(	return result;
}

static const std::string %escaped_name%%unique_id%_name = "%name%";

const std::string& Function<%signature%, %name%>::GetName() const
{
	return %escaped_name%%unique_id%_name;
}

Object Function<%signature%, %name%>::Invoke(const std::vector<Object>& args)
{
	if (args.size() != GetParameterCount())
	{
		throw Exception("Invoke(): bad argument count.");
	}
)";

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
	struct %escaped_name%%unique_id%_registrar
	{
		%escaped_name%%unique_id%_registrar()
		{
			::reflang::registry::internal::Register(
					std::make_unique<Function<%signature%, %name%>>());
		}
	} %escaped_name%%unique_id%_instance;
}
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
				{"%signature%", GetFunctionSignature(f)},
				{"%arg_count%", to_string(f.Arguments.size())},
				{"%return_type%", f.ReturnType},
				{"%call_function%", CallFunction(f)},
				{"%escaped_name%", GetNameWithoutColons(f.GetFullName())},
				{"%unique_id%", GetUniqueSuffixForString(f.GetFullName())}
			});
}
