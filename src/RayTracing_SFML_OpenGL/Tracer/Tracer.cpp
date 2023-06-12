#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    // --- Config

    uint32_t max_reflections = 2;

    // --- Data

    tracer::Sphere sphere_1 {{ 55.f,  500.f,   55.f}, 25.f, { 0.5f, 0.0f, 0.0f, 1.f }};
    tracer::Sphere sphere_2 {{  0.f,  500.f,   55.f}, 25.f, { 1.0f, 1.0f, 1.0f, 1.f }};
    tracer::Sphere sphere_3 {{  0.f,  500.f,    0.f}, 25.f, { 0.5f, 0.5f, 0.5f, 1.f }};
    tracer::Sphere sphere_4 {{  0.f,    0.f, -500.f}, 25.f, { 0.0f, 0.0f, 1.0f, 1.f }};

    // --- //

    gears::Color trace_ray(const Ray& ray)
    {
        if (ray.reflections > max_reflections)
            return { 0.f, 0.f, 0.f, 1.f };

        if (auto intr = sphere_intersection(ray, sphere_1))
        {
            auto point_on_sphere = glm::normalize(intr.value() - sphere_1.position);
            auto my_color = gears::Color{ 0.5f + 0.5f * (point_on_sphere.x), 0.5f + 0.5f * (point_on_sphere.z), 0.5f - 0.5f * (point_on_sphere.y) , 1.f };

            if (auto new_ray = reflect_from_sphere(ray, sphere_1))
                return my_color * 0.8f + trace_ray(new_ray.value()) * 0.2f;
        }
        if (auto intr = sphere_intersection(ray, sphere_2))
        {
            // auto intr = glm::normalize(sphere_intersection(ray, sphere_2) - sphere_2.position);

            // return { 0.0f, 0.5f + 0.5f * (intr.z), 0.f , 1.f };

            if (auto new_ray = reflect_from_sphere(ray, sphere_2))
                return sphere_2.color * 0.5f + trace_ray(new_ray.value()) * 0.5f;
        }
        if (auto intr = sphere_intersection(ray, sphere_3))
        {
            return sphere_3.color;
        }
        if (auto intr = sphere_intersection(ray, sphere_4))
        {
            return sphere_4.color;
        }

        return sky_intersection();
    }

    glm::mat3 rotate_matrix(float angle_Z, float angle_X)
    {
        float su = std::sinf(angle_X);
        float cu = std::cosf(angle_X);
        float sv = std::sinf(0.f);
        float cv = std::cosf(0.f);
        float sw = std::sinf(-angle_Z);
        float cw = std::cosf(-angle_Z);
        
        return glm::mat3 {
            cv * cw,
            su * sv * cw - cu * sw,
            su * sw + cu * sv * cw,
            cv * sw,
            cu * cw + su * sv * sw,
            cu * sv * sw - su * cw,
            -sv,
            su * cv,
            cu * cv,
        };
    }


    gears::Color sky_intersection()
    {
        return { 100.f / 255, 150.f / 255, 200.f / 255, 1.f };
    }

    bool intersects_sphere(const Ray& ray, const Sphere& sphere)
    {
        auto distance = ray.origin - sphere.position;

        float b = glm::dot(ray.direction, distance);
        float c = glm::dot(distance, distance) - sphere.radius * sphere.radius;
        float delta = b * b - c;

        if (delta > 0)
        {
            float t1 = -b + sqrtf(delta);
            float t2 = -b - sqrtf(delta);

            if (t1 > 0 && t2 > 0)
                return true;
        }
        return false;
    }
    std::optional<Ray> reflect_from_sphere(Ray ray, const Sphere& sphere)
    {
        ++ray.reflections;

        auto touch_point = sphere_intersection(ray, sphere);

        if (!touch_point)
            return std::nullopt;

        glm::vec3 normal = glm::normalize(touch_point.value() - sphere.position);

        ray.direction = glm::normalize(glm::reflect(ray.direction, normal));

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
                return ray.origin + ray.direction * (t1 < t2 ? t1 : t2);
        }

        return std::nullopt;
    }

} // namespace Tracer
