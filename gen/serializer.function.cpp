#include "serializer.function.hpp"

#include <algorithm>
#include <sstream>

#include "serializer.util.hpp"

using namespace std;
using namespace reflang;

void serializer::SerializeFunction(ostream& o, const Function& f)
{
	stringstream tmpl;
	tmpl << R"(template <>
class Function<decltype(%name%), %name%> : public IFunction
{
	int num_args() const override
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
		if (args.size() != this->num_args())
		{
			throw std::invalid_argument("count");
		})";
	if (f.ReturnType == "void")
	{
		tmpl << R"(
		%name%();
		return Object();)";
	}
	else
	{
		tmpl << R"(
		return Object(%name%());)";
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
	string name_without_colons = f.GetFullName();
	std::replace(
			name_without_colons.begin(), name_without_colons.end(), ':', '_');

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", f.GetFullName()},
				{"%arg_count%", to_string(f.Arguments.size())},
				{"%name_without_colons%", name_without_colons}
			});
}
