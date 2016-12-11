#include "reflang.hpp"
using namespace reflang;

bool Object::is_void() const
{
	return id_ == GetTypeId<void>();
}

int Object::global_id = 0;
