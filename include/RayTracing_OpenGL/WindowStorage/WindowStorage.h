#pragma once
#include <execution>
#include <algorithm>
#include <string>

#include <RayTracing_OpenGL/gears/Functions.h>

#include <RayTracing_OpenGL/Window/Window.h>
#include <RayTracing_OpenGL/Renderer/Renderer.h>
#include <RayTracing_OpenGL/Interface/Interface.h>
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class WindowStorage
{
public:
    WindowStorage(const std::string& window_title);
    ~WindowStorage();

    WindowStorage() = delete;
    WindowStorage(WindowStorage& other) = delete;
    WindowStorage(WindowStorage&& other) = delete;

    bool operator=(const WindowStorage& other) const = delete;
    bool operator=(const WindowStorage&& other) const = delete;

    int run();

private:
    void init();
    void shutdown();

    // --- IO

    void poll_events();
    void on_resize();
    void process_inputs();

    // --- Components

    DataStorage data_storage_;

    Window window_;
    Renderer renderer_;
    Interface interface_;
};
