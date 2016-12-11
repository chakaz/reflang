#include "serializer.function.hpp"
using namespace std;
using namespace reflang;

void serializer::SerializeFunction(ostream& o, const Function& f)
{
	o << "/*\n";
	o << "Function '" << f.GetFullName() << "':\n";
	o << "> " << f.ReturnType << " " << f.Name << "(";
	for (const auto& arg : f.Arguments)
	{
		o << arg.Type << " " << arg.Name << ", ";
	}
	o << ");\n";
	o << "*/\n";
}
