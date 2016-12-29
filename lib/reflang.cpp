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

	unordered_map<string, unique_ptr<IEnum>>& GetEnumsMap()
	{
		static unordered_map<string, unique_ptr<IEnum>> enums;
		return enums;
	}

	unordered_map<string, unique_ptr<IClass>>& GetClassesMap()
	{
		static unordered_map<string, unique_ptr<IClass>> classes;
		return classes;
	}

	string NormalizeTypeName(const string& name)
	{
		const string ignored_prefix = "::";

		if (name.compare(0, ignored_prefix.size(), ignored_prefix) == 0)
		{
			return name.substr(ignored_prefix.size());
		}
		else
		{
			return name;
		}
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
:	deleter_(NoOp)
{
	*this = move(o);
}

Object& Object::operator=(Object&& o)
{
	// Release existing resource if any.
	deleter_();

	id_ = o.id_;
	data_ = o.data_;
	deleter_ = move(o.deleter_);
	o.deleter_ = NoOp;
	return *this;
}

template <>
Object IFunction::operator()<>()
{
	return this->Invoke({});
}

template <>
Object IMethod::operator()<>(const Reference& o)
{
	return this->Invoke(o, {});
}

bool Object::IsVoid() const
{
	return id_ == GetTypeId<void>();
}

Reference::Reference(const Reference& o) = default;
Reference& Reference::operator=(const Reference& o) = default;

atomic<int> reflang::global_id;

vector<IType*> registry::GetByName(const string& raw_name)
{
	vector<IType*> types;
	for (const auto& f : GetFunctionByName(raw_name))
	{
		types.push_back(f);
	}

	auto* c = GetClassByName(raw_name);
	if (c != nullptr)
	{
		types.push_back(c);
	}

	auto* e = GetEnumByName(raw_name);
	if (e != nullptr)
	{
		types.push_back(e);
	}

	return types;
}

vector<IFunction*> registry::GetFunctionByName(const string& raw_name)
{
	string name = NormalizeTypeName(raw_name);
	auto range = GetFunctionsMap().equal_range(name);

	vector<IFunction*> functions;
	functions.reserve(distance(range.first, range.second));
	for (auto it = range.first; it != range.second; ++it)
	{
		functions.push_back(it->second.get());
	}
	return functions;
}

IClass* registry::GetClassByName(const string& raw_name)
{
	string name = NormalizeTypeName(raw_name);
	auto it = GetClassesMap().find(name);
	if (it == GetClassesMap().end())
	{
		return nullptr;
	}
	else
	{
		return it->second.get();
	}
}

IEnum* registry::GetEnumByName(const string& raw_name)
{
	string name = NormalizeTypeName(raw_name);
	auto it = GetEnumsMap().find(name);
	if (it == GetEnumsMap().end())
	{
		return nullptr;
	}
	else
	{
		return it->second.get();
	}
}

void registry::internal::Register(unique_ptr<IFunction>&& f)
{
	GetFunctionsMap().insert(make_pair(f->GetName(), move(f)));
}

void registry::internal::Register(unique_ptr<IClass>&& c)
{
	GetClassesMap().insert(make_pair(c->GetName(), move(c)));
}

void registry::internal::Register(unique_ptr<IEnum>&& e)
{
	GetEnumsMap().insert(make_pair(e->GetName(), move(e)));
}

Exception::Exception(string error)
:	what_(move(error))
{
}

const char* Exception::what() const noexcept
{
	return what_.c_str();
}
