#include "serializer.class.hpp"

#include <sstream>

#include "serializer.util.hpp"

using namespace std;
using namespace reflang;

namespace
{
	string IterateMembers(const Class& c)
	{
		stringstream tmpl;

		for (const auto& member : c.Members)
		{
			tmpl << "		t(c." << member.Name << ");\n";
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
	// Calls T::operator() on each member of '%name%'.
	// Works well with C++14 generic lambdas.
	template <typename T>
	static void IterateMembers(const %name%& c, T t)
	{
%iterate_members%	}

	template <typename T>
	static void IterateMembers(%name%& c, T t)
	{
%iterate_members%	}

	static const constexpr int MemberCount = %member_count%;

	int GetMemberCount() const override
	{
		return MemberCount;
	}
};

const int Class<%name%>::MemberCount;
)";

	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", c.GetFullName()},
				{"%iterate_members%", IterateMembers(c)},
				{"%member_count%", to_string(c.Members.size())},
				{"%name_without_colons%", GetNameWithoutColons(c.GetFullName())}
			});
}
