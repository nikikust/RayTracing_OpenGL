#pragma once
#include <RayTracing_OpenGL/gears/Functions.h>
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Window
{
public:
    Window(DataStorage& data_storage, const std::string window_title);
    Window() = delete;
    Window(Window&) = delete;
    Window(Window&&) = delete;

    ~Window();

    // --- //
    void close();

    void on_resize();

    bool has_focus();
    bool is_open();

    glm::ivec2 get_mouse_pos();
    void set_cursor_position(glm::vec2 position);
    void hide_mouse();
    void show_mouse();

    void cls();
    void flip();

private:
    void create(const std::string window_title);

    void init_GLFW();
    void create_GLFW_window(const std::string window_title);
    void init_GLAD();
    void set_callbacks();


    DataStorage& data_storage_;
    GLFWwindow* window_;

};
