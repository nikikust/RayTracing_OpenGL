#include <RayTracing_OpenGL/Renderer/Tracer/Tracer.h>


namespace tracer
{
    glm::mat3 rotation_matrix(float angle_Z, float angle_X)
    {
        float su = sinf(angle_X);
        float cu = cosf(angle_X);
        float sv = sinf(0.f);
        float cv = cosf(0.f);
        float sw = sinf(-angle_Z);
        float cw = cosf(-angle_Z);
        
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


    gears::Color sky_intersection(const Ray& ray, bool is_sun)
    {
        if (is_sun)
            return { 255.f / 255, 255.f / 255, 200.f / 255, 1.f };

        glm::vec4 zenith_color { .4f, .6f, .8f, 1.f };
        glm::vec4 horizon_color{ 1.f, 1.f, 1.f, 1.f};

        return glm::mix(horizon_color, zenith_color, sqrtf(abs(ray.direction.z)));
    }
    float light_intensity(gears::LookAt normal, gears::LookAt sun_direction)
    {
        return glm::max(glm::dot(normal, -sun_direction), 0.f); // / 2.f + 0.5f;
    }

    bool intersects_sphere(const Ray& ray, const Sphere& sphere)
    {
        // auto distance = ray.origin - sphere.position;
        // 
        // float b = glm::dot(ray.direction, distance);
        // float c = glm::dot(distance, distance) - sphere.radius * sphere.radius;
        // float delta = b * b - c;
        // 
        // if (delta > 0)
        // {
        //     float t1 = -b + sqrtf(delta);
        //     float t2 = -b - sqrtf(delta);
        // 
        //     if (t1 > 0 && t2 > 0)
        //         return true;
        // }
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
        // gears::Origin distance = ray.origin - sphere.position;
        // 
        // float b = glm::dot(ray.direction, distance);
        // float c = glm::dot(distance, distance) - sphere.radius * sphere.radius;
        // float delta = b * b - c;
        // 
        // if (delta > 0)
        // {
        //     float t1 = -b + sqrtf(delta);
        //     float t2 = -b - sqrtf(delta);
        // 
        //     if (t1 > 0 && t2 > 0)
        //     {
        //         auto touch_point = ray.origin + ray.direction * (t1 < t2 ? t1 : t2);
        // 
        //         return HitInfo{ touch_point, glm::normalize(touch_point - sphere.position) };
        //     }
        // }
        return std::nullopt;
    }

} // namespace tracer
