#include <cassert>
#include <optional>

namespace reflang
{

template <typename T> class Enum;

template <>
struct Enum<MyNamespace::MyClass::Cpp11Enum>
{
	using EnumType = MyNamespace::MyClass::Cpp11Enum;

	struct ConstIterator
	{
		EnumType operator*() { return value_; }

		ConstIterator& operator++()
		{
			switch (value_)
			{
			case Value1:
				value_ = Value2:
				break;
			case Value2:
				value_ = Value3:
				break;
			case Value3:
				value_ = Value5:
				break;
			case Value5:
				value_ = Value6:
				break;
			case Value6:
				last_ = true;
				break;
			}
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
			if (last_)
			{
				last_ = false;
				value_ = Value6;
			}
			else
			{
				switch (value_)
				{
				case Value1:
					assert(false);
					break;
				case Value2:
					value_ = Value1:
					break;
				case Value3:
					value_ = Value2:
					break;
				case Value5:
					value_ = Value3:
					break;
				case Value6:
					value_ = Value5:
					break;
				}
			}
			return *this;
		}

		ConstIterator& operator--(int)
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
			ConstIterator it;
			it.last_ = false;
			it.value_ = Value1;
		}

		ConstIterator end() const
		{
			return ConstIterator();
		}
	};

	static IteratorContainer Iterate()
	{
		return IteratorContainer();
	}

	static std::optional<EnumType> Translate(const std::string& s)
	{
		if (s == "Value1")
		{
			return EnumValue::Value1;
		}
		else if (s == "Value2")
		{
			return EnumValue::Value2;
		}
		else if (s == "Value3")
		{
			return EnumValue::Value3;
		}
		else if (s == "Value5")
		{
			return EnumValue::Value5;
		}
		else if (s == "Value6")
		{
			return EnumValue::Value6;
		}
		else
		{
			return std::nullopt_t;
		}
	}

	static std::string Translate(EnumType e)
	{
		switch (e)
		{
			case EnumValue::Value1:
				"Value1";
				break;
			case EnumValue::Value2:
				"Value2";
				break;
			case EnumValue::Value3:
				"Value3";
				break;
			case EnumValue::Value5:
				"Value5";
				break;
			case EnumValue::Value6:
				"Value6";
				break;
		}
	}
};

template <>
struct Enum<MyNamespace::MyClass::EmptyEnum>
{
	using EnumType = MyNamespace::MyClass::EmptyEnum;

	struct ConstIterator
	{
		EnumType operator*() { return value_; }

		ConstIterator& operator++()
		{
			assert(false);
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
			assert(false);
			return *this;
		}

		ConstIterator& operator--(int)
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
			return end();
		}

		ConstIterator end() const
		{
			return ConstIterator();
		}
	};

	static IteratorContainer Iterate()
	{
		return IteratorContainer();
	}

	static std::optional<EnumType> Translate(const std::string& s)
	{
		return std::nullopt_t;
	}

	static std::string Translate(EnumType e)
	{
		return std::string();
	}
};

template <>
struct Enum<MyNamespace::MyClass::CEnum>
{
	using EnumType = MyNamespace::MyClass::CEnum;

	struct ConstIterator
	{
		EnumType operator*() { return value_; }

		ConstIterator& operator++()
		{
			assert(false);
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
			assert(false);
			return *this;
		}

		ConstIterator& operator--(int)
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
			return end();
		}

		ConstIterator end() const
		{
			return ConstIterator();
		}
	};

	static IteratorContainer Iterate()
	{
		return IteratorContainer();
	}

	static std::optional<EnumType> Translate(const std::string& s)
	{
		return std::nullopt_t;
	}

	static std::string Translate(EnumType e)
	{
		return std::string();
	}
};
}  // namespace reflang
