#pragma once
#include <string>

#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


class WindowStorage
{
public:
    WindowStorage(const std::wstring& window_title);

    WindowStorage() = delete;
    WindowStorage(WindowStorage& other) = delete;
    WindowStorage(WindowStorage&& other) = delete;
    
    bool operator==(const WindowStorage& other) const = delete;

    void shutdown();
    void pollEvents();
    bool isRunning();

    void ImGui_init();
    void ImGui_update();
    void ImGui_render();
    void ImGui_shutdown();

    bool window_has_focus();

    void window_cls();
    void window_flip();

    void render_view();

private:
    sf::RenderWindow window_;
    sf::Clock deltaClock_;

    bool running_ = true;

    sf::Font font_;

    // --- Render data

    gears::Position camera_position {  0.f,  0.f,  0.f };
    gears::Angles   camera_angles   {  0.f,  1.f,  0.f };
    gears::Angles   sun_angle       {  0.f,  0.f, -1.f };

    float vFOV_half = 50.f / 2;
    float hFOV_half = vFOV_half * (16.f / 9.f);
};
