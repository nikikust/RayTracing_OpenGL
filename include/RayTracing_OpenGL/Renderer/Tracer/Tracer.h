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

    glm::mat3 rotation_matrix(float angle_Z, float angle_X);

    gears::Color sky_intersection(const Ray& ray, bool is_sun);
    float light_intensity(gears::LookAt normal, gears::LookAt sun_direction);

    bool intersects_sphere(const Ray& ray, const Sphere& sphere);
    Ray reflect(Ray ray, const HitInfo& hit_info);
    std::optional<HitInfo> sphere_intersection(const Ray& ray, const Sphere& sphere);
};