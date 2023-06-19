#pragma once
#include <optional>
#include <vector>

#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/Renderer/Tracer/Objects.h>


namespace tracer
{
	struct HitInfo
	{
		gears::Origin hit_origin;
		gears::LookAt normal;
	};

	gears::Color trace_ray(const Ray& ray);
	glm::mat3 rotate_matrix(float angle_Z, float angle_X);

	gears::Color sky_intersection(const Ray& ray);
	float light_intensity(gears::LookAt normal);

	bool intersects_sphere(const Ray& ray, const Sphere& sphere);
	Ray reflect(Ray ray, const HitInfo& hit_info);
	std::optional<HitInfo> sphere_intersection(const Ray& ray, const Sphere& sphere);

};
