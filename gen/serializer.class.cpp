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

	string IterateStaticFields(const Class& c)
	{
		stringstream tmpl;

		for (const auto& field : c.StaticFields)
		{
			tmpl << "	t(" << c.GetFullName() << "::" << field.Name << ");\n";
		}

		return tmpl.str();
	}
}

void serializer::SerializeClassHeader(ostream& o, const Class& c)
{
	stringstream tmpl;
	tmpl << R"(
template <>
class Class<%name%> : public IClass
{
public:
	static const constexpr int FieldCount = %field_count%;
	static const constexpr int StaticFieldCount = %static_field_count%;

	int GetFieldCount() const override;
	int GetStaticFieldCount() const override;

	const std::string& GetName() const override;

	// Calls T::operator() on each field of '%name%'.
	// Works well with C++14 generic lambdas.
	template <typename T>
	static void IterateFields(const %name%& c, T t);

	template <typename T>
	static void IterateFields(%name%& c, T t);

	template <typename T>
	static void IterateStaticFields(T t);
};

template <typename T>
void Class<%name%>::IterateFields(const %name%& c, T t)
{
%iterate_fields%}

template <typename T>
void Class<%name%>::IterateFields(%name%& c, T t)
{
%iterate_fields%}

template <typename T>
void Class<%name%>::IterateStaticFields(T t)
{
%iterate_static_fields%}
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", c.GetFullName()},
				{"%iterate_fields%", IterateFields(c)},
				{"%iterate_static_fields%", IterateStaticFields(c)},
				{"%field_count%", to_string(c.Fields.size())},
				{"%static_field_count%", to_string(c.StaticFields.size())},
			});
}

void serializer::SerializeClassSources(ostream& o, const Class& c)
{
	stringstream tmpl;
	tmpl << R"(
const int Class<%name%>::FieldCount;
const int Class<%name%>::StaticFieldCount;

int Class<%name%>::GetFieldCount() const
{
	return FieldCount;
}

int Class<%name%>::GetStaticFieldCount() const
{
	return StaticFieldCount;
}

static const std::string %escaped_name%_name = "%name%";

const std::string& Class<%name%>::GetName() const
{
	return %escaped_name%_name;
}
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", c.GetFullName()},
				{"%iterate_fields%", IterateFields(c)},
				{"%field_count%", to_string(c.Fields.size())},
				{"%escaped_name%", GetNameWithoutColons(c.GetFullName())}
			});
}
