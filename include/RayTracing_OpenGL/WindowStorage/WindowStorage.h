#pragma once
#include <execution>
#include <algorithm>
#include <string>

#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/Tracer/Tracer.h>


class WindowStorage
{
public:
    WindowStorage(const std::wstring& window_title);
    ~WindowStorage();

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

    sf::Vector2u get_view_area();
    sf::Time get_frame_elapsed_time();

    // --- IO

    tracer::Camera& get_camera();

    void set_cursor_position(sf::Vector2u position);
    void hide_mouse();
    void show_mouse();

private:
    // --- Internal data

    sf::RenderWindow window_;
    sf::Clock deltaClock_;
    sf::Time time_elapsed_;

    sf::Vector2u screen_size;

    bool running_ = true;

    sf::Font font_;

    // --- Render data

    tracer::Camera camera = { { 0.f, 0.f, 0.f}, {0.f, 0.f} };

    float vFOV_half = 50.f / 2.f * (glm::pi<float>() / 180.f);
    float screen_ratio;

    std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;

    gears::Vertex* frame_buffer_;
};
