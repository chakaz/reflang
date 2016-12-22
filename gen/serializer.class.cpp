#include "serializer.class.hpp"

#include <sstream>

#include "serializer.util.hpp"

using namespace std;
using namespace reflang;

namespace
{
	string IterateFields(const Class& c)
	{
		stringstream tmpl;

		for (const auto& field : c.Fields)
		{
			tmpl << "		t(c." << field.Name << ");\n";
		}

		return tmpl.str();
	}
}

void serializer::SerializeClass(ostream& o, const Class& c)
{
	stringstream tmpl;
	tmpl << R"(template <>
class Class<%name%> : public IClass
{
public:
	static const constexpr int FieldCount = %field_count%;

	int GetFieldCount() const override
	{
		return FieldCount;
	}

	const std::string& GetName() const override
	{
		static const std::string name = "%name%";
		return name;
	}

	// Calls T::operator() on each field of '%name%'.
	// Works well with C++14 generic lambdas.
	template <typename T>
	static void IterateFields(const %name%& c, T t)
	{
%iterate_fields%	}

	template <typename T>
	static void IterateFields(%name%& c, T t)
	{
%iterate_fields%	}
};

const int Class<%name%>::FieldCount;
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", c.GetFullName()},
				{"%iterate_fields%", IterateFields(c)},
				{"%field_count%", to_string(c.Fields.size())},
				{"%name_without_colons%", GetNameWithoutColons(c.GetFullName())}
			});
}
