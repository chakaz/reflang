#ifndef REFLANG_TYPES_HPP
#define REFLANG_TYPES_HPP

#include <atomic>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace reflang
{
	// Object is a class similar in nature to std::any, only it does not require
	// C++17 and does not require stored objects to be copyable.
	class Object final
	{
	public:
		// Constructs an Object with stored type of void.
		Object();

		// Constructs an Object with a copy of T.
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
		bool IsT() const
		{
			return GetTypeId<T>() == id_;
		}

		template <typename T>
		const T& GetT() const
		{
			if (GetTypeId<T>() != id_)
			{
				throw std::invalid_argument("Can't cast to T.");
			}
			return *static_cast<T*>(data_);
		}

		bool IsVoid() const;

	private:
		static std::atomic<int> global_id;

		template <typename T>
		static int GetTypeId()
		{
			// While it may seem like global_id could be an int rather than
			// std::atomic, this is in fact no true. GetTypeId<T1> does not
			// sync access to GetTypeId<T2>.
			static const int t_id = global_id++;
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

		virtual int GetParameterCount() const = 0;

		// Syntactic sugar for calling Invoke().
		template <typename... Ts>
		Object operator()(Ts&&... ts)
		{
			Object init[] = { Object(std::forward<Ts>(ts))... };
			std::vector<Object> v(
					std::make_move_iterator(std::begin(init)),
					std::make_move_iterator(std::end(init)));
			return this->Invoke(std::move(v));
		}

		virtual Object Invoke(const std::vector<Object>& args) = 0;
	};

	template <>
	Object IFunction::operator()<>();

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
