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
			tmpl << "	t(c." << field.Name << ");\n";
		}

		return tmpl.str();
	}

	string SerializeHeader(const Class& c)
	{
		stringstream tmpl;
		tmpl << R"(
template <>
class Class<%name%> : public IClass
{
public:
	static const constexpr int FieldCount = %field_count%;

	int GetFieldCount() const override;

	const std::string& GetName() const override;

	// Calls T::operator() on each field of '%name%'.
	// Works well with C++14 generic lambdas.
	template <typename T>
	static void IterateFields(const %name%& c, T t);

	template <typename T>
	static void IterateFields(%name%& c, T t);
};

template <typename T>
void Class<%name%>::IterateFields(const %name%& c, T t)
{
%iterate_fields%}

template <typename T>
void Class<%name%>::IterateFields(%name%& c, T t)
{
%iterate_fields%}
)";

		return serializer::ReplaceAll(
				tmpl.str(),
				{
					{"%name%", c.GetFullName()},
					{"%iterate_fields%", IterateFields(c)},
					{"%field_count%", to_string(c.Fields.size())},
				});
	}

	string SerializeSource(const Class& c)
	{
		stringstream tmpl;
		tmpl << R"(
const int Class<%name%>::FieldCount;

int Class<%name%>::GetFieldCount() const
{
	return FieldCount;
}

static const std::string %escaped_name%_name = "%name%";

const std::string& Class<%name%>::GetName() const
{
	return %escaped_name%_name;
}
)";

		return serializer::ReplaceAll(
				tmpl.str(),
				{
					{"%name%", c.GetFullName()},
					{"%iterate_fields%", IterateFields(c)},
					{"%field_count%", to_string(c.Fields.size())},
					{"%escaped_name%", serializer::GetNameWithoutColons(c.GetFullName())}
				});
	}
}

void serializer::SerializeClass(ostream& o, const Class& c)
{
	o << SerializeHeader(c);
	o << SerializeSource(c);
}
