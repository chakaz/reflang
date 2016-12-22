#ifndef RETURN_NOARGS_SRC_HPP
#define RETURN_NOARGS_SRC_HPP

class DummyClass {};

namespace ns
{
	const char* NamespacedFunction();
}

const char* GlobalFunction();

DummyClass ReturnByValue();

const DummyClass& ReturnByReference();

#endif //RETURN_NOARGS_SRC_HPP
