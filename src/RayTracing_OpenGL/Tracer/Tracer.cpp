#include <RayTracing_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    // --- Config

    uint32_t max_reflections = 2;

    gears::LookAt sun_angle = glm::normalize(gears::LookAt{  -1.f, 0.f, -1.f });

    // --- Data

    std::vector<tracer::Sphere> spheres{
        tracer::Sphere{{ 55.f, 500.f,  55.f}, 25.f, { 0.05f, { 0.5f, 0.0f, 0.0f, 1.f } }},
        tracer::Sphere{{  0.f, 500.f,  55.f}, 25.f, { 0.75f, { 1.0f, 1.0f, 1.0f, 1.f } }},
        tracer::Sphere{{  0.f, 500.f,   0.f}, 25.f, { 0.15f, { 0.5f, 0.5f, 0.5f, 1.f } }},
        tracer::Sphere{{  0.f, 0.f,   500.f}, 25.f, { 0.00f, { 0.0f, 0.0f, 1.0f, 1.f } }}
    };

    // --- //

    gears::Color trace_ray(const Ray& ray)
    {
        if (ray.reflections > max_reflections)
            return { 0.f, 0.f, 0.f, 1.f };

        float min_hit_distance = FLT_MAX;
        gears::Color res_color = sky_intersection(ray);

        for (auto& sphere : spheres)
        {
            if (auto intr = sphere_intersection(ray, sphere))
            {
                float curr_distance = glm::length(intr.value().hit_origin - ray.origin);
                if (curr_distance < min_hit_distance)
                {
                    auto diff = sphere.material.color * light_intensity(intr.value().normal);
                    if (sphere.material.albedo == 0.f)
                        res_color = diff;
                    else
                    {
                        auto new_ray = reflect(ray, intr.value());

                        res_color = glm::mix(diff, trace_ray(new_ray), sphere.material.albedo);
                    }
                    min_hit_distance = curr_distance;
                }
            }
        }

        return res_color;
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


    gears::Color sky_intersection(const Ray& ray)
    {
        if (glm::dot(-sun_angle, ray.direction) > 0.999f)
            return { 255.f / 255, 255.f / 255, 200.f / 255, 1.f };
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
