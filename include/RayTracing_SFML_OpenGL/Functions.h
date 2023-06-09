#pragma once
#include <cstddef>


inline namespace literals
{
	char const* operator"" _C(const char8_t* str, std::size_t);
}