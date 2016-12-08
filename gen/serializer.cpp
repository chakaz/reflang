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

	vector<string> GetEnumValues(const Enum& the_enum)
	{
		vector<string> results;

		for (const auto& it : the_enum.Values)
		{
			results.push_back(it.first);
		}

		return results;
	}

	void SerializeEnum(ostream& o, const Enum& the_enum)
	{
		vector<string> unique_values = GetEnumUniqueValues(the_enum);
		vector<string> values = GetEnumValues(the_enum);

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
)";
		if (unique_values.empty())
		{
			o << "			assert(false);\n";
		}
		else
		{
			o << R"(			switch (value_)
			{
)";
			for (int i = 1; i < unique_values.size(); ++i)
			{
				const auto& prev = unique_values[i - 1];
				const auto& it = unique_values[i];
				o << "			case EnumType::" << prev << ":\n";
				o << "				value_ = EnumType::" << it << ";\n";
				o << "				break;\n";
			}
			o << "			case EnumType::" << unique_values.back() << ":\n";
			o << R"(				last_ = true;
				break;
			}
)";
		}
		o << R"(			return *this;
		}

		ConstIterator operator++(int)
		{
			auto tmp = *this;
			operator++();
			return tmp;
		}

		ConstIterator& operator--()
		{
)";
		if (unique_values.empty())
		{
			o << "			assert(false);\n";
		}
		else
		{
			o << R"(			if (last_)
			{
				last_ = false;
				)";
				o << "value_ = EnumType::" << unique_values.back() << ";\n";
			o << R"(			}
			else
			{
				switch (value_)
				{
)";
			o << "				case EnumType::" << unique_values.front() << ":\n";
			o << "					assert(false);\n";
			o << "					break;\n";
			for (int i = 1; i < unique_values.size(); ++i)
			{
				const auto& prev = unique_values[i - 1];
				const auto& it = unique_values[i];
				o << "				case EnumType::" << it << ":\n";
				o << "					value_ = EnumType::" << prev << ";\n";
				o << "					break;\n";
			}
			o << "				}\n";
			o << "			}\n";
		}
		o << R"(			return *this;
		}

		ConstIterator operator--(int)
		{
			auto tmp = *this;
			operator--();
			return tmp;
		}

		bool operator==(const ConstIterator& o) const
		{
			return ((last_ && o.last_) || (value_ == o.value_));
		}

		bool operator!=(const ConstIterator& o) const
		{
			return !(*this == o);
		}

		EnumType value_;
		bool last_ = true;
	};

	struct IteratorContainer
	{
		ConstIterator begin() const
		{
)";
		if (unique_values.empty())
		{
			o << "			return end();\n";
		}
		else
		{
			o << R"(			ConstIterator it;
			it.last_ = false;
			it.value_ = EnumType::)" << unique_values.front() << R"(;
			return it;
)";
		}
		o << R"(		}

		ConstIterator end() const
		{
			return ConstIterator();
		}
	};

	static IteratorContainer Iterate()
	{
		return IteratorContainer();
	}

	static bool TryTranslate(const std::string& s, EnumType& value)
	{
)";
		if (values.empty())
		{
			o << "		return false;\n";
		}
		else
		{
			for (size_t i = 0; i < values.size(); ++i)
			{
				o << "		";
				if (i != 0)
				{
					o << "else ";
				}
				o << "if (s == \"" << values[i] << "\")\n";
				o << "		{\n";
				o << "			value = EnumType::" << values[i] <<";\n";
				o << "			return true;\n";
				o << "		}\n";
			}
			o << R"(		else
		{
			return false;
		}
)";
		}
		o << R"(	}

	static std::string Translate(EnumType e)
	{
)";
		if (unique_values.empty())
		{
			o << "		return std::string();\n";
		}
		else
		{
			o << "		switch (e)\n";
			o << "		{\n";
			for (const auto& value : unique_values)
			{
				o << "			case EnumType::" << value << ":\n";
				o << "				return \"" << value <<"\";\n";
				o << "				break;\n";
			}
			o << "		}\n";
		}
		o << R"(	}
};
)";
	}
}  // namespace

void serializer::Begin(ostream& o)
{
	o << R"(#include <cassert>
#include <string>

namespace reflang
{

template <typename T> class Enum;
)";
}

void serializer::Serialize(ostream& o, const TypeBase& type)
{
	switch (type.GetType())
	{
		case TypeBase::Type::Enum:
			SerializeEnum(o, static_cast<const Enum&>(type));
			break;
	}
}

void serializer::End(ostream& o)
{
	o << "}  // namespace reflang\n";
}
