#include "reflang.hpp"

#include <iterator>
#include <string>
#include <unordered_map>

using namespace reflang;
using namespace std;

namespace
{
	void noop() {}

	unordered_multimap<string, unique_ptr<IFunction>>& GetFunctionsMap()
	{
		static unordered_multimap<string, unique_ptr<IFunction>> functions;
		return functions;
	}
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

void registry::internal::Register(unique_ptr<IFunction>&& function)
{
	GetFunctionsMap().insert(make_pair(function->GetName(), move(function)));
}
