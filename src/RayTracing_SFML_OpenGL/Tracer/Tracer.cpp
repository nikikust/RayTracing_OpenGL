#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    // --- Config

    uint32_t max_reflections = 8;

    // --- //

    gears::Color trace_ray(const Ray& ray)
    {
        return sky_intersection();
    }


    gears::Color sky_intersection()
    {
        return { 100.f / 255, 150.f / 255, 200.f / 255, 1.f };
    }

    bool intersects_sphere(const Ray& ray, const Sphere& sphere)
    {
        return false;
    }
    Ray reflect_from_sphere(const Ray& ray, const Sphere& sphere)
    {
        return ray;
    }
    gears::Position sphere_intersection(const Ray& ray, const Sphere& sphere)
    {
        return sphere.position;
    }

} // namespace Tracer
