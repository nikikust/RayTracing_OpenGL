#pragma once
#include <optional>
#include <vector>

#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/Renderer/Tracer/Objects.h>


namespace tracer
{
    glm::mat3 rotation_matrix(float angle_Z, float angle_X);
};
