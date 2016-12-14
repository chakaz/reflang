#include "serializer.util.hpp"

#include <regex>

using namespace reflang;
using namespace std;

string serializer::ReplaceAll(
		const string& text,
		initializer_list<FromToPair> pairs)
{
	string result = text;
	for (const auto& pair : pairs)
	{
		regex replace(pair.first);
		result = regex_replace(result, replace, pair.second);
	}
	return result;
}
