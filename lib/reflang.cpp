#include "reflang.hpp"
using namespace reflang;

namespace
{
	void noop() {}
}

Object::Object()
:	id_(GetTypeId<void>())
,	deleter_(noop)
{
}

Object::~Object()
{
	deleter_();
}

Object::Object(Object&& o)
{
	*this = std::move(o);
}

Object& Object::operator=(Object&& o)
{
	id_ = o.id_;
	data_ = o.data_;
	deleter_ = std::move(o.deleter_);
	o.deleter_ = noop;
	return *this;
}

bool Object::is_void() const
{
	return id_ == GetTypeId<void>();
}

int Object::global_id = 0;
