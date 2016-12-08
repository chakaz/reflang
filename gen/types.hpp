#ifndef REFLANG_TYPES_H
#define REFLANG_TYPES_H

#include <string>
#include <vector>

namespace reflang
{
	class TypeBase
	{
	public:
		enum class Type
		{
			Enum,
		};

	public:
		TypeBase(std::string full_name);
		virtual ~TypeBase();

		virtual Type GetType() const = 0;
		const std::string& GetFullName() const;
		const std::string& GetName() const;

	private:
		std::string full_name_;
	};

	class Enum : public TypeBase
	{
	public:
		using ValueList = std::vector<std::pair<std::string, int>>;
		
	public:
		Enum(std::string full_name);
		Type GetType() const override;

		ValueList Values;
	};
}

#endif //REFLANG_TYPES_H
