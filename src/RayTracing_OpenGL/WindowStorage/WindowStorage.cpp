#include <RayTracing_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(const std::string& window_title)
    : window_   (data_storage_, window_title),
      renderer_ (data_storage_),
      interface_(data_storage_)
{
    init();
}
WindowStorage::~WindowStorage()
{
    shutdown();
}

// --- //

int WindowStorage::run()
{
    while (window_.is_open())
    {
        poll_events();
        process_inputs();

        renderer_ .update();
        interface_.update();

        window_.cls();
        renderer_ .draw();
        interface_.draw();
        window_.flip();
    }

    return 0;
}

void WindowStorage::init()
{
    data_storage_.last_mouse_position = window_.get_mouse_pos();
}
void WindowStorage::shutdown()
{

}

// --- //

void WindowStorage::process_inputs()
{
    using namespace gears;
    auto screen_size = data_storage_.screen_size;
    
    data_storage_.current_mouse_position = window_.get_mouse_pos();
    data_storage_.mouse_position_delta = data_storage_.current_mouse_position - data_storage_.last_mouse_position;

    if (key_hit(GLFW_KEY_ESCAPE))
        window_.close();

    if (key_hit(GLFW_KEY_R))
    {
        data_storage_.camera.origin = { 0.f, 0.f, 0.f };
        data_storage_.camera.angles = { 0.f, 0.f };
    }

    if (key_hit(GLFW_KEY_LEFT_ALT))
        data_storage_.capture_IO = !data_storage_.capture_IO;

    if (data_storage_.capture_IO)
    {
        window_.set_cursor_position({ screen_size.x / 2, screen_size.y / 2 });
        data_storage_.current_mouse_position = window_.get_mouse_pos();

        window_.hide_mouse();
        data_storage_.camera.rotate(data_storage_.mouse_position_delta.x / 500.f, data_storage_.mouse_position_delta.y / -500.f);

        float delta_speed = (float)
            (data_storage_.deltaTime * 1000 * data_storage_.moving_speed *
                (gears::key_down(GLFW_KEY_LEFT_SHIFT) ? 2.f : 1.f));

        if (gears::key_down(GLFW_KEY_SPACE))
            data_storage_.camera.move_up(delta_speed);
        if (gears::key_down(GLFW_KEY_LEFT_CONTROL))
            data_storage_.camera.move_down(delta_speed);
        if (gears::key_down(GLFW_KEY_W))
            data_storage_.camera.move_forward(delta_speed);
        if (gears::key_down(GLFW_KEY_S))
            data_storage_.camera.move_backward(delta_speed);
        if (gears::key_down(GLFW_KEY_D))
            data_storage_.camera.move_right(delta_speed);
        if (gears::key_down(GLFW_KEY_A))
            data_storage_.camera.move_left(delta_speed);
    }
    else
    {
        window_.show_mouse();
    }

    // --- //

    data_storage_.last_mouse_position = data_storage_.current_mouse_position;
}

// --- Event methods

void WindowStorage::on_resize()
{
    window_  .on_resize();
    renderer_.on_resize();
}
void WindowStorage::poll_events()
{
    glfwPollEvents();

    if (gears::global_flags[0] == true)
    {
        on_resize();
        gears::global_flags[0] = 0;
    }

    double nowTime = glfwGetTime();
    data_storage_.deltaTime = nowTime - data_storage_.lastTime;
    data_storage_.lastTime = nowTime;

    gears::ignore_input = !window_.has_focus();
}
