#pragma once
#include <string>
#include <vector>
#include <future>
#include <array>

#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/gears/StaticThreadPool.hpp>

#define THREADPOOL_SIZE 4


class WindowStorage
{
public:
    WindowStorage(std::wstring window_title);

    WindowStorage() = delete;
    WindowStorage(WindowStorage& other) = delete;
    WindowStorage(WindowStorage&& other) = delete;
    
    ~WindowStorage();

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

    void trace_rays(const glm::vec2& start, const glm::vec2& stop_x, const glm::vec2& size, sf::RenderWindow& window) const;

private:
    sf::RenderWindow window_;
    sf::Clock deltaClock_;

    bool running_ = true;

    sf::Font font_;

    gears::StaticThreadPool stp_;

    mutable std::mutex gl_mutex_;
};
