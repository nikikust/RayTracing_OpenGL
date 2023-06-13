#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


int main()
{
    // --- Initialization --- //

    using namespace gears;

    WindowStorage window_storage("Ray Tracing");
    window_storage.ImGui_init();

    bool capture_IO = false;

    glm::ivec2 current_mouse_position{};
    glm::ivec2 mouse_position_delta{};
    glm::ivec2 last_mouse_position{ window_storage.get_mouse_pos() };

    // --- Main Loop --- //

    while (window_storage.isRunning())
    {
        // --- Process OS Window

        window_storage.pollEvents();

        ignore_input = !window_storage.window_has_focus();

        window_storage.ImGui_update();

        // --- Logic

        glm::ivec2 current_mouse_position{ window_storage.get_mouse_pos() };
        glm::ivec2 mouse_position_delta = current_mouse_position - last_mouse_position;
        last_mouse_position = current_mouse_position;
        
        auto screen_size = window_storage.get_view_area();
        
        if (key_hit(GLFW_KEY_ESCAPE))
            window_storage.close_window();
        
        if (key_hit(GLFW_KEY_R))
        {
            window_storage.get_camera().origin = { 0.f, 0.f, 0.f };
            window_storage.get_camera().angles = { 0.f, 0.f };
        }
        
        if (key_hit(GLFW_KEY_LEFT_ALT))
            capture_IO = !capture_IO;
        
        if (capture_IO)
        {
            window_storage.set_cursor_position({ screen_size.x / 2, screen_size.y / 2 });
            current_mouse_position = { window_storage.get_mouse_pos() };
        
            window_storage.hide_mouse();
            window_storage.get_camera().rotate(mouse_position_delta.x / 250.f, mouse_position_delta.y / -250.f);
        
            window_storage.process_inputs();
        }
        else
        {
            window_storage.show_mouse();
        }
        
        last_mouse_position = current_mouse_position;
        
        // --- Windows
        
        ImGui::Begin("Information");
        
        ImGui::Text("Screen resolution: %ix%i", screen_size.x, screen_size.y);
        ImGui::Separator();
        ImGui::Text("Camera X: %.1f", window_storage.get_camera().origin.x);
        ImGui::Text("Camera Y: %.1f", window_storage.get_camera().origin.y);
        ImGui::Text("Camera Z: %.1f", window_storage.get_camera().origin.z);
        ImGui::Separator();
        ImGui::Text("Camera U: %.2f", window_storage.get_camera().angles.x);
        ImGui::Text("Camera V: %.2f", window_storage.get_camera().angles.y);
        ImGui::Separator();
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        
        ImGui::End();

        // --- Render
        window_storage.window_cls();
        window_storage.render_view();
        window_storage.ImGui_render();
        window_storage.window_flip();
    }

    window_storage.ImGui_shutdown();
    window_storage.shutdown();

    return 0;
}