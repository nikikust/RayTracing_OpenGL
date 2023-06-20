#pragma once
#include <vector>


#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/Renderer/Tracer/Objects.h>


class DataStorage
{
public:
	DataStorage();
	DataStorage(DataStorage&) = delete;
	DataStorage(DataStorage&&) = delete;

    // --- Data --- //

    glm::ivec2 screen_size{};

    double lastTime, deltaTime;

    bool capture_IO = false;
    bool hide_mouse = false;

    glm::ivec2 current_mouse_position{};
    glm::ivec2 mouse_position_delta{};
    glm::ivec2 last_mouse_position{};

    // --- Render data

    tracer::Camera camera = { { 0.f, 0.f, 0.f}, {0.f, 0.f} };

    float vFOV_half = 50.f / 2.f * (glm::pi<float>() / 180.f);
    float screen_ratio = 1;
    float moving_speed = 0.1f;

    uint32_t max_reflections = 2;

    gears::LookAt sun_angle = glm::normalize(gears::LookAt{  -1.f, 1.f, -1.f });

    // --- Objects data

    std::vector<tracer::Sphere> spheres{
        tracer::Sphere{{ 55.f, 500.f,  55.f}, 25.f, { 0.05f, { 0.5f, 0.0f, 0.0f, 1.f } }},
        tracer::Sphere{{  0.f, 500.f,  55.f}, 25.f, { 0.75f, { 1.0f, 1.0f, 1.0f, 1.f } }},
        tracer::Sphere{{  0.f, 500.f,   0.f}, 25.f, { 0.15f, { 0.5f, 0.5f, 0.5f, 1.f } }},
        tracer::Sphere{{  0.f,   0.f, 500.f}, 25.f, { 0.00f, { 0.0f, 0.0f, 1.0f, 1.f } }}
    };
};
