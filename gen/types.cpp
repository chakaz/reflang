#include "types.hpp"
using namespace reflang;

TypeBase::TypeBase(std::string full_name)
:	full_name_(std::move(full_name))
{
}

TypeBase::~TypeBase() = default;

const std::string& TypeBase::GetFullName() const
{
	return full_name_;
}

const std::string& TypeBase::GetName() const
{
	return full_name_;
}

Enum::Enum(std::string full_name)
:	TypeBase(std::move(full_name))
{
}

TypeBase::Type Enum::GetType() const
{
	return Type::Enum;
}
