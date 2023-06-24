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
} // namespace tracer
