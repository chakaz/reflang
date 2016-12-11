#include "serializer.hpp"

#include <fstream>

#include "serializer.enum.hpp"
#include "serializer.class.hpp"

using namespace std;
using namespace reflang;

namespace
{
	void Begin(ostream& o, const serializer::Options& options)
	{
		o << R"(#include <cassert>
#include <string>

)";
		o << options.include_path;
		o << R"(

namespace reflang
{

template <typename T> class Enum;
template <typename T> class Class;

)";
	}

	void End(ostream& o)
	{
		o << "}  // namespace reflang\n";
	}
}  // namespace

void serializer::Serialize(
		const std::vector<std::unique_ptr<TypeBase>>& types,
		const Options& options)
{
	std::unique_ptr<ofstream> fout;
	ostream* o = &cout;

	if (!options.out_hpp_path.empty())
	{
		fout = make_unique<ofstream>(options.out_hpp_path.c_str());
		o = fout.get();
	}

	Begin(*o, options);
	for (const auto& type : types)
	{
		switch (type->GetType())
		{
			case TypeBase::Type::Enum:
				SerializeEnum(*o, static_cast<const Enum&>(*type));
				break;
			case TypeBase::Type::Class:
				SerializeClass(*o, static_cast<const Class&>(*type));
				break;
		}
	}
	End(*o);
}
