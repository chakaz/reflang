#include <cassert>
#include <optional>

namespace reflang
{

template <typename T> class Enum;

template <>
struct Enum<MyNamespace::MyClass::MyEnum>
{
	using EnumType = MyNamespace::MyClass::MyEnum;

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
	}

	static std::string Translate(EnumType e)
	{
	}
};

template <>
struct Enum<MyNamespace::MyClass::MyEnum2>
{
	using EnumType = MyNamespace::MyClass::MyEnum2;

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
	}

	static std::string Translate(EnumType e)
	{
	}
};
}  // namespace reflang
