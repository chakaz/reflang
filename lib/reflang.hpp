#ifndef REFLANG_TYPES_HPP
#define REFLANG_TYPES_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace reflang
{
	class Object final
	{
	public:
		Object();

		template <typename T>
		explicit Object(T&& t)
		:	id_(GetTypeId<std::decay_t<T>>())
		,	data_(new std::decay_t<T>(std::forward<T>(t)))
		{
			// This is not part of the initializer list because it
			// doesn't compile on VC.
			deleter_ =
				[this]()
				{
					delete static_cast<std::decay_t<T>*>(data_);
				};
		}

		Object(Object&& o);
		Object& operator=(Object&& o);
		Object(const Object& o) = delete;
		Object& operator=(const Object& o) = delete;

		~Object();

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

		int id_;
		void* data_ = nullptr;
		std::function<void()> deleter_;
	};

	class IEnum
	{
	public:
		virtual ~IEnum() = default;

		virtual const std::string& GetName() const = 0;

		virtual std::vector<std::string> GetStringValues() const = 0;
		virtual std::vector<int> GetIntValues() const = 0;
		virtual bool TryTranslate(const std::string& value, int& out) = 0;
		virtual bool TryTranslate(int value, std::string& out) = 0;
	};

	class IFunction
	{
	public:
		virtual ~IFunction() = default;

		virtual const std::string& GetName() const = 0;

		virtual int num_args() const = 0;

		template <typename... Ts>
		Object operator()(Ts&&... ts)
		{
			return this->Invoke({ Object(ts)... });
		}

		virtual Object Invoke(const std::vector<Object>& args) = 0;
	};

	namespace registry
	{
		std::vector<IFunction*> GetFunctionByName(const std::string& name);
		std::vector<IEnum*> GetEnumByName(const std::string& name);

		namespace internal
		{
			void Register(std::unique_ptr<IFunction>&& f);
			void Register(std::unique_ptr<IEnum>&& e);
		}
	};

	// Below is implementation helpers.
	template <typename T> class Enum;
	template <typename T> class Class;
	template <typename T, T t> class Function;

}

#endif //REFLANG_TYPES_HPP
