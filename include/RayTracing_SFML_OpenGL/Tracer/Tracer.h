#pragma once
#include <RayTracing_SFML_OpenGL/gears/Functions.h>


namespace tracer
{
	using position = glm::vec3;
	using angles   = glm::vec2;


	void trace_ray(const glm::vec2& direction);


	gears::Color sky_intersection();

	gears::Color sphere_intersection();
};
