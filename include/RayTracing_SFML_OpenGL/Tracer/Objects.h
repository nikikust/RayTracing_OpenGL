#pragma once
#include <RayTracing_SFML_OpenGL/gears/Functions.h>

namespace tracer
{
	struct Camera
	{
		gears::Origin origin;
		gears::Angles angles;

		void rotate(float x, float y);

		void move_forward(float a);
		void move_backward(float a);
		void move_left(float a);
		void move_right(float a);
		void move_up(float a);
		void move_down(float a);
	};

	struct Ray
	{
		gears::Origin origin;
		gears::LookAt direction;
		gears::Color color;

		uint32_t reflections;

		Ray(const gears::Origin& origin, const gears::LookAt& direction, const gears::Color& color = { 0.f, 0.f, 0.f, 1.f }, const uint32_t& reflections = 0)
			: origin(origin), direction(direction), color(color), reflections(reflections) {}
	};

	struct Sphere
	{
		gears::Origin position;
		float radius;

		gears::Color color;
	};

} // namespace tracer