#include "serializer.enum.hpp"

#include <memory>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "serializer.util.hpp"

using namespace std;
using namespace reflang;

namespace
{
	vector<string> GetEnumUniqueValues(const Enum& e)
	{
		vector<string> results;
		unordered_set<int> used_values;

		for (const auto& it : e.Values)
		{
			if (used_values.find(it.second) == used_values.end())
			{
				used_values.insert(it.second);
				results.push_back(it.first);
			}
		}

		return results;
	}

	vector<string> GetEnumValues(const Enum& e)
	{
		vector<string> results;

		for (const auto& it : e.Values)
		{
			results.push_back(it.first);
		}

		return results;
	}
}

void serializer::SerializeEnumHeader(ostream& o, const Enum& e)
{
	stringstream tmpl;
	tmpl << R"(template <>
struct Enum<%name%> : public IEnum
{
	using EnumType = %name%;

	struct ConstIterator
	{
		ConstIterator(bool is_last);

		EnumType operator*();

		ConstIterator& operator++();
		ConstIterator operator++(int);
		ConstIterator& operator--();
		ConstIterator operator--(int);

		bool operator==(const ConstIterator& o) const;
		bool operator!=(const ConstIterator& o) const;

	private:
		EnumType value_;
		bool last_ = true;
	};

	struct IteratorContainer
	{
		ConstIterator begin() const;
		ConstIterator end() const;
	};

	static IteratorContainer Iterate();

	static bool TryTranslate(const std::string& s, EnumType& value);
	static std::string Translate(EnumType e);

	const std::string& GetName() const override;

	std::vector<std::string> GetStringValues() const override;

	std::vector<int> GetIntValues() const override;

	bool TryTranslate(const std::string& value, int& out) override;
	bool TryTranslate(int value, std::string& out) override;
};
)";
	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", e.GetFullName()}
			});
}

void serializer::SerializeEnumSources(ostream& o, const Enum& e)
{
	vector<string> unique_values = GetEnumUniqueValues(e);
	vector<string> values = GetEnumValues(e);

	stringstream tmpl;
	tmpl << R"(
Enum<%name%>::ConstIterator::ConstIterator(bool is_last)
)";
	if (!unique_values.empty())
	{
		tmpl << ":	last_(is_last)\n";
		tmpl << ",	value_(EnumType::" << unique_values.front() << ")";
	}
	else {
		tmpl << ":	last_(true)";
	}
	tmpl << R"(
{
}

Enum<%name%>::EnumType Enum<%name%>::ConstIterator::operator*()
{
	return value_;
}

Enum<%name%>::ConstIterator& Enum<%name%>::ConstIterator::operator++()
{
)";
	if (unique_values.empty())
	{
		tmpl << "	assert(false);\n";
	}
	else
	{
		tmpl << R"(	switch (value_)
	{
)";
		for (size_t i = 1; i < unique_values.size(); ++i)
		{
			const auto& prev = unique_values[i - 1];
			const auto& it = unique_values[i];
			tmpl << "		case EnumType::" << prev << ":\n";
			tmpl << "			value_ = EnumType::" << it << ";\n";
			tmpl << "			break;\n";
		}
		tmpl << "		case EnumType::" << unique_values.back() << ":\n";
		tmpl << R"(			last_ = true;
			break;
	}
)";
	}
	tmpl << R"(	return *this;
}

Enum<%name%>::ConstIterator Enum<%name%>::ConstIterator::operator++(int)
{
	auto tmp = *this;
	operator++();
	return tmp;
}

Enum<%name%>::ConstIterator& Enum<%name%>::ConstIterator::operator--()
{
)";
	if (unique_values.empty())
	{
		tmpl << "	assert(false);\n";
	}
	else
	{
		tmpl << R"(	if (last_)
	{
		last_ = false;
		value_ = EnumType::)" << unique_values.back() << R"(;
	}
	else
	{
		switch (value_)
		{
)";
		tmpl << "			case EnumType::" << unique_values.front() << ":\n";
		tmpl << "				assert(false);\n";
		tmpl << "				break;\n";
		for (size_t i = 1; i < unique_values.size(); ++i)
		{
			const auto& prev = unique_values[i - 1];
			const auto& it = unique_values[i];
			tmpl << "			case EnumType::" << it << ":\n";
			tmpl << "				value_ = EnumType::" << prev << ";\n";
			tmpl << "				break;\n";
		}
		tmpl << "		}\n";
		
		tmpl << "	}\n";
	}
	tmpl << R"(	return *this;
}

Enum<%name%>::ConstIterator Enum<%name%>::ConstIterator::operator--(int)
{
	auto tmp = *this;
	operator--();
	return tmp;
}

bool Enum<%name%>::ConstIterator::operator==(const ConstIterator& o) const
{
	return ((last_ && o.last_) ||
		(!last_ && !o.last_ && value_ == o.value_));
}

bool Enum<%name%>::ConstIterator::operator!=(const ConstIterator& o) const
{
	return !(*this == o);
}

Enum<%name%>::ConstIterator Enum<%name%>::IteratorContainer::begin() const
{
	return ConstIterator(false);
}

Enum<%name%>::ConstIterator Enum<%name%>::IteratorContainer::end() const
{
	return ConstIterator(true);
}

Enum<%name%>::IteratorContainer Enum<%name%>::Iterate()
{
	return IteratorContainer();
}

bool Enum<%name%>::TryTranslate(const std::string& s, EnumType& value)
{
)";
	if (values.empty())
	{
		tmpl << "	return false;\n";
	}
	else
	{
		for (size_t i = 0; i < values.size(); ++i)
		{
			tmpl << "	";
			if (i != 0)
			{
				tmpl << "else ";
			}
			tmpl << "if (s == \"" << values[i] << "\")\n";
			tmpl << "	{\n";
			tmpl << "		value = EnumType::" << values[i] <<";\n";
			tmpl << "		return true;\n";
			tmpl << "	}\n";
		}
		tmpl << R"(	else
	{
		return false;
	}
)";
	}
	tmpl << R"(}

std::string Enum<%name%>::Translate(EnumType e)
{
)";
	if (unique_values.empty())
	{
		tmpl << "	return std::string();\n";
	}
	else
	{
		tmpl << "	switch (e)\n";
		tmpl << "	{\n";
		for (const auto& value : unique_values)
		{
			tmpl << "		case EnumType::" << value << ":\n";
			tmpl << "			return \"" << value <<"\";\n";
			tmpl << "			break;\n";
		}
		tmpl << "	}\n";
		tmpl << "	return std::string();\n";
	}
	tmpl << R"(}

static const std::string %escaped_name%_name = "%name%";

const std::string& Enum<%name%>::GetName() const
{
	return %escaped_name%_name;
}

std::vector<std::string> Enum<%name%>::GetStringValues() const
{
	std::vector<std::string> values;
	values.reserve(%unique_values_count%);
	for (const auto& value : this->Iterate())
	{
		values.push_back(this->Translate(value));
	}
	return values;
}

std::vector<int> Enum<%name%>::GetIntValues() const
{
	std::vector<int> values;
	values.reserve(%unique_values_count%);
	for (const auto& value : this->Iterate())
	{
		values.push_back(static_cast<int>(value));
	}
	return values;
}

bool Enum<%name%>::TryTranslate(const std::string& value, int& out)
{
	EnumType tmp;
	bool result = this->TryTranslate(value, tmp);
	if (result)
	{
		out = static_cast<int>(tmp);
	}
	return result;
}

bool Enum<%name%>::TryTranslate(int value, std::string& out)
{
)";
	if (unique_values.empty())
	{
		tmpl << "	return false;\n";
	}
	else
	{
		tmpl << R"(	switch (static_cast<EnumType>(value))
	{
)";
		for (const auto& value : unique_values)
		{
			tmpl << "	case EnumType::" << value << ":\n";
		}
		if (!unique_values.empty())
		{
			tmpl << R"(		out = Translate(static_cast<EnumType>(value));
		return true;
)";
		}
		tmpl << R"(	default:
		return false;
	}
)";
	}
	tmpl << R"(}

namespace
{
	struct %escaped_name%_registrar
	{
		%escaped_name%_registrar()
		{
			::reflang::registry::internal::Register(
				std::make_unique<Enum<%name%>>());
		}
	} %escaped_name%_instance;
}
)";
	o << ReplaceAll(
			tmpl.str(),
			{
				{"%name%", e.GetFullName()},
				{"%unique_values_count%", to_string(unique_values.size())},
				{"%escaped_name%", GetNameWithoutColons(e.GetFullName())}
			});
}
