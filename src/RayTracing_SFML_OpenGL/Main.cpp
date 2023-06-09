#include <RayTracing_SFML_OpenGL/gears/Functions.h>
#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


int main()
{
    // --- Initialization --- //

    WindowStorage window_storage(L"Трасировка Лучей");
    window_storage.ImGui_init();

    // --- Main Loop --- //

    while (window_storage.isRunning())
    {
        // --- Process OS Window

        window_storage.pollEvents();

        ignore_input = !window_storage.window_has_focus();

        window_storage.ImGui_update();

        // --- Logic

        

        // --- Render
        window_storage.window_cls();
        window_storage.render_view();
        window_storage.ImGui_render();
        window_storage.window_flip();
    }

    window_storage.ImGui_shutdown();

    return 0;
}