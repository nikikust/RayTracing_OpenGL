#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


int main()
{
    // --- Initialization --- //

    using namespace gears;

    WindowStorage window_storage(L"Трассировка Лучей");
    window_storage.ImGui_init();

    sf::Vector2i last_mouse_position{ sf::Mouse::getPosition() };

    bool capture_IO = false;
    float moving_speed = 4.f;

    // --- Main Loop --- //

    while (window_storage.isRunning())
    {
        // --- Process OS Window

        window_storage.pollEvents();

        ignore_input = !window_storage.window_has_focus();

        window_storage.ImGui_update();

        // --- Logic

        sf::Vector2i current_mouse_position{ sf::Mouse::getPosition() };
        sf::Vector2i mouse_position_delta = current_mouse_position - last_mouse_position;
        auto screen_size = window_storage.get_view_area();

        if (key_hit(sf::Keyboard::Escape))
            window_storage.shutdown();

        if (key_hit(sf::Keyboard::R))
        {
            window_storage.get_camera().origin = { 0.f, 0.f, 0.f };
            window_storage.get_camera().angles = { 0.f, 0.f };
        }

        if (key_hit(sf::Keyboard::LAlt))
            capture_IO = !capture_IO;

        if (capture_IO)
        {
            window_storage.set_cursor_position({ screen_size.x / 2, screen_size.y / 2 });
            current_mouse_position = { sf::Mouse::getPosition() };

            window_storage.hide_mouse();
            window_storage.get_camera().rotate(mouse_position_delta.x / 250.f, mouse_position_delta.y / -250.f);

            if (key_down(sf::Keyboard::Space))
                window_storage.get_camera().move_up(moving_speed);
            if (key_down(sf::Keyboard::LControl))
                window_storage.get_camera().move_down(moving_speed);
            if (key_down(sf::Keyboard::W))
                window_storage.get_camera().move_forward(moving_speed);
            if (key_down(sf::Keyboard::S))
                window_storage.get_camera().move_backward(moving_speed);
            if (key_down(sf::Keyboard::D))
                window_storage.get_camera().move_right(moving_speed);
            if (key_down(sf::Keyboard::A))
                window_storage.get_camera().move_left(moving_speed);
        }
        else
        {
            window_storage.show_mouse();
        }

        last_mouse_position = current_mouse_position;

        // --- Windows

        ImGui::Begin("Information");

        ImGui::Text("Camera X: %.1f", window_storage.get_camera().origin.x);
        ImGui::Text("Camera Y: %.1f", window_storage.get_camera().origin.y);
        ImGui::Text("Camera Z: %.1f", window_storage.get_camera().origin.z);
        ImGui::Text("");
        ImGui::Text("Camera U: %.2f", window_storage.get_camera().angles.x);
        ImGui::Text("Camera V: %.2f", window_storage.get_camera().angles.y);
        
        ImGui::End();

        // --- Render
        window_storage.window_cls();
        window_storage.render_view();
        window_storage.ImGui_render();
        window_storage.window_flip();
    }

    window_storage.ImGui_shutdown();

    return 0;
}