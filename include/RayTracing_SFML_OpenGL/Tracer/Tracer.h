#pragma once
#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/Tracer/Objects.h>


namespace tracer
{
	gears::Color trace_ray(const Ray& ray);


	gears::Color sky_intersection();

	bool intersects_sphere(const Ray& ray, const Sphere& sphere);
	Ray reflect_from_sphere(const Ray& ray, const Sphere& sphere);
	gears::Position sphere_intersection(const Ray& ray, const Sphere& sphere);

};
