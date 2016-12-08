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
			last_ = false;
			switch (value_)
			{
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
}  // namespace reflang
