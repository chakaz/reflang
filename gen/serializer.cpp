#include "serializer.hpp"

#include <unordered_set>

using namespace std;
using namespace reflang;

namespace
{
	vector<string> GetEnumUniqueValues(const Enum& the_enum)
	{
		vector<string> results;
		unordered_set<int> used_values;

		for (const auto& it : the_enum.Values)
		{
			if (used_values.find(it.second) == used_values.end())
			{
				used_values.insert(it.second);
				results.push_back(it.first);
			}
		}

		return results;
	}

	void SerializeEnum(ostream& o, const Enum& the_enum)
	{
		vector<string> values = GetEnumUniqueValues(the_enum);

		const string& name = the_enum.GetFullName();
		o << R"(
template <>
struct Enum<)" << name << R"(>
{
	using EnumType = )" << name << R"(;

	struct ConstIterator
	{
		EnumType operator*() { return value_; }

		ConstIterator& operator++()
		{
			switch (value_)
			{
)";
		for (int i = 1; i < values.size(); ++i)
		{
			const auto& prev = values[i - 1];
			const auto& it = values[i];
			o << "			case " << prev << ":\n";
			o << "				value_ = " << it << ":\n";
			o << "				break;\n";
		}
		if (!values.empty())
		{
			o << "			case " << values.back() << ":\n";
			o << "				last_ = true;\n";
			o << "				break;\n";
		}
		o << R"(			}
			return *this;
		}

		ConstIterator& operator++(int)
		{
			auto tmp = *this;
			operator++();
			return tmp;
		}

		ConstIterator& operator--()
		{
			last_ = false;
			switch (value_)
			{
)";
		for (int i = 1; i < values.size(); ++i)
		{
			const auto& prev = values[i - 1];
			const auto& it = values[i];
			o << "			case " << it << ":\n";
			o << "				value_ = " << prev << ":\n";
			o << "				break;\n";
		}
		o << R"(			}
			return *this;
		}

		ConstIterator& operator--(int)
		{
			auto tmp = *this;
			operator--();
			return tmp;
		}

	private:
		EnumType value_;
		bool last_ = true;
	};

	struct IteratorContainer
	{
		ConstIterator begin() const;
		ConstIterator end() const;
	};

	static IteratorContainer Iterate()
	{
	}
};
)";
	}
}  // namespace

ostream& serializer::Serialize(ostream& o, const TypeBase& type)
{
	o << R"(namespace reflang
{

template <typename T> class Enum;
)";

	switch (type.GetType())
	{
		case TypeBase::Type::Enum:
			SerializeEnum(o, static_cast<const Enum&>(type));
			break;
	}

	o << "}  // namespace reflang\n";
	return o;
}
