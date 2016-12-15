#ifndef REFLANG_CMDARGS_HPP
#define REFLANG_CMDARGS_HPP

#include <functional>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace reflang
{
	class CmdArgs
	{
	public:
		class Exception
		{
		public:
			Exception(const std::string& error);

			std::string GetError() const;

		private:
			std::string error_;
		};

		template <typename U>
		inline static U Convert(const std::string& s);

		template <typename T>
		class Arg
		{
		public:
			T Get() const
			{
				return value_;
			}

			void Set(const std::string& value)
			{
				value_ = Convert<T>(value);
			}

		private:
			Arg(T default_value)
			:	value_(default_value)
			{
			}

			T value_;
			friend class CmdArgs;
		};

		template <typename T>
		std::shared_ptr<Arg<T>> Register(
				const std::string& cmd,
				const std::string& help,
				T default_value)
		{
			auto arg = std::make_shared<Arg<T>>(Arg<T>(default_value));
			if (args_.find(cmd) != args_.end())
			{
				throw Exception("Argument of the same cmd already registered.");
			}

			args_[cmd] = 
					{
						help,
						[arg](const std::string& s) { arg->Set(s); }
					};
			return std::move(arg);
		}

		// Consumes arguments until "--".
		// Returns how many arguments were consumed.
		int Consume(int argc, char** argv);

		void PrintHelp() const;

	private:
		struct InternalArg
		{
			std::string help;
			std::function<void(const std::string&)> setter;
		};

		using Args = std::unordered_map<std::string, InternalArg>;
		Args args_;
	};

	template <>
	inline std::string CmdArgs::Convert<std::string>(const std::string& s)
	{
		return s;
	}

	template <>
	inline int CmdArgs::Convert<int>(const std::string& s)
	{
		return std::stoi(s);
	}

	template <>
	inline bool CmdArgs::Convert<bool>(const std::string& s)
	{
		if (s == "true")
		{
			return true;
		}
		else if (s == "false")
		{
			return false;
		}
		else
		{
			throw Exception("Unable to parse bool value " + s + ".");
		}
	}
}

#endif //REFLANG_CMDARGS_HPP
