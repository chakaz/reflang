#include "types.hpp"
using namespace reflang;
using namespace std;

TypeBase::TypeBase(string file, string full_name)
:	full_name_(move(full_name))
,	file_(move(file))
{
}

TypeBase::~TypeBase() = default;

const string& TypeBase::GetFullName() const
{
	return full_name_;
}

const string& TypeBase::GetName() const
{
	return full_name_;
}

Enum::Enum(string file, string full_name)
:	TypeBase(move(file), move(full_name))
{
}

Enum::Type Enum::GetType() const
{
	return Type::Enum;
}

Class::Class(string file, string full_name)
:	TypeBase(move(file), move(full_name))
{
}

Class::Type Class::GetType() const
{
	return Type::Class;
}
