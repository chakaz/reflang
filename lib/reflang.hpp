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
		explicit Object(T&& t);

		Object(Object&& o);
		Object& operator=(Object&& o);
		Object(const Object& o) = delete;
		Object& operator=(const Object& o) = delete;

		~Object();

		template <typename T>
		bool IsT() const;

		template <typename T>
		const T& GetT() const;

		bool IsVoid() const;

	private:
		int id_;
		void* data_ = nullptr;
		std::function<void()> deleter_;
	};

	// Reference is a non-const, type erased wrapper around any object.
	class Reference final
	{
	public:
		// Constructs a Reference to t.
		template <typename T>
		Reference(T& t);

		Reference(const Reference& o);
		Reference& operator=(const Reference& o);

		template <typename T>
		bool IsT() const;

		template <typename T>
		T& GetT() const;

	private:
		int id_;
		void* data_ = nullptr;
	};

	extern std::atomic<int> global_id;

	template <typename T>
	int GetTypeId();

	class IType
	{
	public:
		virtual ~IType() = default;

		virtual const std::string& GetName() const = 0;
	};

	class IMethod : public IType
	{
	public:
		virtual int GetParameterCount() const = 0;

		// Syntactic sugar for calling Invoke().
		template <typename... Ts>
		Object operator()(const Reference& o, Ts&&... ts)
		{
			Object init[] = { Object(std::forward<Ts>(ts))... };
			std::vector<Object> v(
					std::make_move_iterator(std::begin(init)),
					std::make_move_iterator(std::end(init)));
			return this->Invoke(o, std::move(v));
		}

		virtual Object Invoke(const Reference& o, const std::vector<Object>& args) = 0;
	};

	class IClass : public IType
	{
	public:
		virtual int GetFieldCount() const = 0;
		//virtual Object GetField(const std::string& name) const = 0;

		virtual int GetStaticFieldCount() const = 0;
		//virtual Object GetStaticField(const std::string& name) const = 0;

		virtual int GetMethodCount() const = 0;
		//virtual std::vector<IMethod*> GetMethods(const std::string& name) const = 0;
	};

	class IEnum : public IType
	{
	public:
		virtual std::vector<std::string> GetStringValues() const = 0;
		virtual std::vector<int> GetIntValues() const = 0;
		virtual bool TryTranslate(const std::string& value, int& out) = 0;
		virtual bool TryTranslate(int value, std::string& out) = 0;
	};

	class IFunction : public IType
	{
	public:
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

	namespace registry
	{
		std::vector<IFunction*> GetFunctionByName(const std::string& name);
		IEnum* GetEnumByName(const std::string& name);

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
	template <typename T, T t> class Method;
}

// Implementation and specializations
template <typename T>
int reflang::GetTypeId()
{
	// While it may seem like global_id could be an int rather than
	// std::atomic, this is in fact no true. GetTypeId<T1> does not
	// sync access with GetTypeId<T2>.
	static const int t_id = global_id++;
	return t_id;
}

template <typename T>
reflang::Object::Object(T&& t)
:	id_(GetTypeId<std::decay_t<T>>())
,	data_(new std::decay_t<T>(std::forward<T>(t)))
{
	// This is not part of the initializer list because it
	// doesn't compile on VC.
	deleter_ = [this]()
	{
		delete static_cast<std::decay_t<T>*>(data_);
	};
}

template <typename T>
bool reflang::Object::IsT() const
{
	return GetTypeId<T>() == id_;
}

template <typename T>
const T& reflang::Object::GetT() const
{
	if (GetTypeId<T>() != id_)
	{
		throw std::invalid_argument("Can't cast to T.");
	}
	return *static_cast<T*>(data_);
}

template <typename T>
reflang::Reference::Reference(T& t)
:	id_(GetTypeId<std::decay_t<T>>())
,	data_(new std::decay_t<T>(std::forward<T>(t)))
{
}

template <typename T>
bool reflang::Reference::IsT() const
{
	return GetTypeId<T>() == id_;
}

template <typename T>
T& reflang::Reference::GetT() const
{
	if (GetTypeId<T>() != id_)
	{
		throw std::invalid_argument("Can't cast to T.");
	}
	return *static_cast<T*>(data_);
}

template <>
reflang::Object reflang::IFunction::operator()<>();
template <>
reflang::Object reflang::IMethod::operator()<>(const Reference& o);

#endif //REFLANG_TYPES_HPP
