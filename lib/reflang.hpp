#ifndef REFLANG_TYPES_HPP
#define REFLANG_TYPES_HPP

#include <vector>

namespace reflang
{
	template <typename T> class Enum;
	template <typename T> class Class;
	template <typename T> class Function;

	class Object final
	{
	public:
		template <typename T>
		Object(T&& t)
		:	id_(GetTypeId<T>())
			//TODO: save value
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
				throw 1; //TODO: throw reflang exception
			}
			return T(); //TODO: return saved value
		}

		bool is_void() const;

	private:
		static int global_id;

		template <typename T>
		int GetTypeId()
		{
			static int t_id = global_id++;
			return t_id;
		}

		const int id_;
	};

	class IFunction
	{
	public:
		virtual ~IFunction() = default;

		virtual int num_args() const = 0;
		virtual Object operator()(const std::vector<Object>& args) = 0;
	};
}

#endif //REFLANG_TYPES_HPP
