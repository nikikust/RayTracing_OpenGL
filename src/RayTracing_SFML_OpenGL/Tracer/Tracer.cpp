#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    // --- Config

    uint32_t max_reflections = 2;

    gears::LookAt sun_angle = glm::normalize(gears::LookAt{  -1.f, 0.f, -1.f });

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
            auto my_color = gears::Color
            { 
                0.5f + 0.5f * (intr.value().normal.x),
                0.5f + 0.5f * (intr.value().normal.z),
                0.5f - 0.5f * (intr.value().normal.y),
                1.f};

            auto new_ray = reflect(ray, intr.value());

            return my_color * 0.8f * light_intensity(intr.value().normal) + trace_ray(new_ray) * 0.2f;
        }
        if (auto intr = sphere_intersection(ray, sphere_2))
        {
            auto new_ray = reflect(ray, intr.value());
            
            return sphere_2.color * 0.5f * light_intensity(intr.value().normal) + trace_ray(new_ray) * 0.5f;
        }
        if (auto intr = sphere_intersection(ray, sphere_3))
        {
            return sphere_3.color * light_intensity(intr.value().normal);
        }
        if (auto intr = sphere_intersection(ray, sphere_4))
        {
            return sphere_4.color * light_intensity(intr.value().normal);
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
    float light_intensity(gears::LookAt normal)
    {
        return glm::dot(normal, -sun_angle) / 2.f + 0.5f;
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
    Ray reflect(Ray ray, const HitInfo& hit_info)
    {
        ++ray.reflections;

        ray.direction = glm::normalize(glm::reflect(ray.direction, hit_info.normal));

        ray.origin = hit_info.hit_origin + ray.direction * 0.1f;
        
        return ray;
    }
    std::optional<HitInfo> sphere_intersection(const Ray& ray, const Sphere& sphere)
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
            {
                auto touch_point = ray.origin + ray.direction * (t1 < t2 ? t1 : t2);

                return HitInfo{ touch_point, glm::normalize(touch_point - sphere.position) };
            }
        }
        return std::nullopt;
    }

} // namespace Tracer
