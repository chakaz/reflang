#include "serializer.class.hpp"
using namespace std;
using namespace reflang;

void serializer::SerializeClass(ostream& o, const Class& c)
{
	o << "/*\n";
	o << "Class '" << c.GetFullName() << "'.\nMethods:\n";
	for (const auto& method : c.Methods)
	{
		o << "> " << method.ReturnType << " " << method.Name << "(";
		for (const auto& arg : method.Arguments)
		{
			o << arg.Type << " " << arg.Name << ", ";
		}
		o << ");\n";
	}
	o << "Members:\n";
	for (const auto& member : c.Members)
	{
		o << "> " << member.Type << " " << member.Name << "\n";
	}
	o << "*/\n";
}
