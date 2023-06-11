#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    position camera_position{ 0.f, 0.f, 0.f };
    angles camera_angles{ 0.f, 0.f };

    float vFOV = 50.f;
    float hFOV = vFOV * (16.f / 9.f);


    void trace_ray(const glm::vec2& direction)
    {
        auto color = sky_intersection();
        glColor3f(color.r, color.g, color.b);
        glVertex2f(direction.x, direction.y);
    }


    gears::Color sky_intersection()
    {
        return { 100.f / 255, 150.f / 255, 200.f / 255, 1.f };
    }

    gears::Color sphere_intersection()
    {
        return { 0.f, 0.f, 0.f, 1.f };
    }

} // namespace Tracer
