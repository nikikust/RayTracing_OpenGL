#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    // --- Config

    uint32_t max_reflections = 1;

    // --- Data

    tracer::Sphere sphere_1 {{55.f, 500.f, 55.f}, 25.f, { 0.5f, 0.0f, 0.0f, 1.f }};
    tracer::Sphere sphere_2 {{ 0.f, 500.f, 55.f}, 25.f, { 0.5f, 0.5f, 0.0f, 1.f }};
    tracer::Sphere sphere_3 {{ 0.f, 500.f,  0.f}, 25.f, { 0.5f, 0.5f, 0.5f, 1.f }};

    // --- //

    gears::Color trace_ray(const Ray& ray)
    {
        if (ray.reflections > max_reflections)
            return { 0.f, 0.f, 0.f, 1.f };

        if (intersects_sphere(ray, sphere_1))
        {
            return sphere_1.color;
        }
        if (intersects_sphere(ray, sphere_2))
        {
            // auto intr = glm::normalize(sphere_intersection(ray, sphere_2) - sphere_2.position);

            // return { 0.0f, 0.5f + 0.5f * (intr.z), 0.f , 1.f };

            if (auto new_ray = reflect_from_sphere(ray, sphere_2))
                return sphere_2.color * 0.0f + trace_ray(new_ray.value()) * 1.0f;
        }
        if (intersects_sphere(ray, sphere_3))
        {
            return sphere_3.color;
        }

        return sky_intersection();
    }


    gears::Color sky_intersection()
    {
        return { 100.f / 255, 150.f / 255, 200.f / 255, 1.f };
    }

    bool intersects_sphere(const Ray& ray, const Sphere& sphere)
    {
        auto distance = ray.origin - sphere.position;

        float b = 2 * glm::dot(ray.direction, distance);
        float c = glm::length(distance) * glm::length(distance) - sphere.radius * sphere.radius;
        float delta = b * b - 4 * c;

        return delta > 0;
    }
    std::optional<Ray> reflect_from_sphere(Ray ray, const Sphere& sphere)
    {
        ++ray.reflections;

        auto touch_point = sphere_intersection(ray, sphere);

        if (!touch_point)
            return std::nullopt;

        if (touch_point == ray.origin - ray.direction)
            return ray;

        auto norm = glm::normalize(touch_point.value() - sphere.position);

        ray.direction = glm::reflect(ray.direction, norm);

        ray.origin = touch_point.value() + ray.direction * 0.1f;
        
        return ray;
    }
    std::optional<gears::Origin> sphere_intersection(const Ray& ray, const Sphere& sphere)
    {
        auto distance = ray.origin - sphere.position;

        float b = 2 * glm::dot(ray.direction, distance);
        float c = glm::length(distance) * glm::length(distance) - sphere.radius * sphere.radius;
        float delta = b * b - 4 * c;

        if (delta > 0)
        {
            float t1 = (-b + sqrtf(delta)) / 2.f;
            float t2 = (-b - sqrtf(delta)) / 2.f;

            if (t1 > 0 && t2 > 0)
                return ray.origin + glm::normalize(ray.direction) * (t1 < t2 ? t1 : t2);
        }

        return std::nullopt;
    }

} // namespace Tracer
