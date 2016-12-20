#include "reflang.hpp"

#include <iterator>
#include <string>
#include <unordered_map>

using namespace reflang;
using namespace std;

namespace
{
	void NoOp() {}

	unordered_multimap<string, unique_ptr<IFunction>>& GetFunctionsMap()
	{
		static unordered_multimap<string, unique_ptr<IFunction>> functions;
		return functions;
	}

	unordered_multimap<string, unique_ptr<IEnum>>& GetEnumsMap()
	{
		static unordered_multimap<string, unique_ptr<IEnum>> enums;
		return enums;
	}
}

Object::Object()
:	id_(GetTypeId<void>())
,	deleter_(NoOp)
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
	o.deleter_ = NoOp;
	return *this;
}

template <>
Object IFunction::operator()<>()
{
	return this->Invoke({});
}

bool Object::IsVoid() const
{
	return id_ == GetTypeId<void>();
}

atomic<int> Object::global_id;

vector<IFunction*> registry::GetFunctionByName(const string& name)
{
	auto range = GetFunctionsMap().equal_range(name);

	vector<IFunction*> functions;
	functions.reserve(distance(range.first, range.second));
	for (auto it = range.first; it != range.second; ++it)
	{
		functions.push_back(it->second.get());
	}
	return functions;
}

vector<IEnum*> registry::GetEnumByName(const string& name)
{
	auto range = GetEnumsMap().equal_range(name);

	vector<IEnum*> enums;
	enums.reserve(distance(range.first, range.second));
	for (auto it = range.first; it != range.second; ++it)
	{
		enums.push_back(it->second.get());
	}
	return enums;
}

void registry::internal::Register(unique_ptr<IFunction>&& f)
{
	GetFunctionsMap().insert(make_pair(f->GetName(), move(f)));
}

void registry::internal::Register(unique_ptr<IEnum>&& e)
{
	GetEnumsMap().insert(make_pair(e->GetName(), move(e)));
}
