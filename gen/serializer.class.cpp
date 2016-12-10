#include "serializer.class.hpp"
using namespace std;
using namespace reflang;

void serializer::SerializeClass(ostream& o, const Class& c)
{
	o << "/*\n";
	o << "Class '" << c.GetFullName() << "' with:\n";
	for (const auto& method : c.Methods)
	{
		o << "> " << method << "()\n";
	}
	o << "*/\n";
}
