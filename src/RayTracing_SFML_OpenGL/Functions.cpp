#include <RayTracing_SFML_OpenGL/Functions.h>


inline namespace literals
{
	char const* operator"" _C(const char8_t* str, std::size_t)
	{
		return reinterpret_cast<const char*>(str);
	}
}