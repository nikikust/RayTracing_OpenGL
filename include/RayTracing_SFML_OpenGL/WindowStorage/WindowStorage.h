#pragma once
#include <string>

#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/gears/StaticThreadPool.hpp>


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

private:
    sf::RenderWindow window_;
    sf::Clock deltaClock_;

    bool running_ = true;

    sf::Font font;

    gears::StaticThreadPool stp;
};
