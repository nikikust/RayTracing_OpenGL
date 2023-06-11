#pragma once
#include <RayTracing_SFML_OpenGL/gears/Functions.h>

namespace tracer
{
	struct Ray
	{
		gears::Position position;
		gears::Angles direction;
		gears::Color color;

		uint32_t reflections;

		Ray(const gears::Position& position, const gears::Angles& direction, const gears::Color& color = { 0.f, 0.f, 0.f, 1.f }, const uint32_t& reflections = 0)
			: position(position), direction(direction), color(color), reflections(reflections) {}
	};

	struct Sphere
	{
		gears::Position position;
		float radius;

		gears::Color color;
	};

} // namespace tracer
