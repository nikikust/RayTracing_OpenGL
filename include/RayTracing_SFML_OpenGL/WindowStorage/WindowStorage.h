#pragma once
#include <execution>
#include <algorithm>
#include <string>

#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/Tracer/Tracer.h>


class WindowStorage
{
public:
    WindowStorage(const std::string& window_title);

    WindowStorage() = delete;
    WindowStorage(WindowStorage& other) = delete;
    WindowStorage(WindowStorage&& other) = delete;

    bool operator=(const WindowStorage& other) const = delete;
    bool operator=(const WindowStorage&& other) const = delete;

    void close_window();
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

    glm::ivec2 get_view_area();
    double get_frame_elapsed_time();

    // --- IO

    tracer::Camera& get_camera();

    void set_cursor_position(glm::vec2 position);
    void hide_mouse();
    void show_mouse();

    glm::ivec2 get_mouse_pos();

    void process_inputs();

private:
    // --- Internal data

    GLFWwindow* window_;

    glm::ivec2 screen_size{};

    double lastTime, deltaTime;

    // --- Render data

    tracer::Camera camera = { { 0.f, 0.f, 0.f}, {0.f, 0.f} };

    float vFOV_half = 50.f / 2.f * (glm::pi<float>() / 180.f);
    float screen_ratio = 1;
    float moving_speed = 0.1f;

    std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;
};
