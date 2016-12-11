#ifndef REFLANG_TYPES_HPP
#define REFLANG_TYPES_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

namespace reflang
{
	class Object final
	{
	public:
		template <typename T>
		Object(T&& t)
		:	id_(GetTypeId<T>())
		,	data_(new T(std::forward<T>(t)))
		,	deleter_(
				[this]()
				{
					delete static_cast<T*>(data_);
				})
		{
		}

		Object()
		:	id_(GetTypeId<void>())
		,	deleter_([]() {})
		{
		}

		template <typename T>
		bool is_t() const
		{
			return GetTypeId<T>() == id_;
		}

		template <typename T>
		T get_t() const
		{
			if (GetTypeId<T>() != id_)
			{
				throw std::invalid_argument("Can't cast to T.");
			}
			return T(*static_cast<T*>(data_));
		}

		bool is_void() const;

	private:
		static int global_id;

		template <typename T>
		static int GetTypeId()
		{
			static int t_id = global_id++;
			return t_id;
		}

		const int id_;
		void* data_ = nullptr;
		std::function<void()> deleter_;
	};

	class IFunction
	{
	public:
		virtual ~IFunction() = default;

		virtual int num_args() const = 0;

		virtual Object operator()(const std::vector<Object>& args) = 0;
	};

	// Below is implementation helpers.
	template <typename T> class Enum;
	template <typename T> class Class;

	template <typename R, typename... Args>
	struct FunctionIdWrapper
	{
		template <R F(Args...)>
		struct FunctionId {};
	};
	template <typename T, T t> class Function;

}

#endif //REFLANG_TYPES_HPP
