#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <RayTracing_SFML_OpenGL/Functions.h>


int main()
{
    // --- Initialization --- //

    sf::Window main_window(sf::VideoMode(400u, 400u), L"Трасировка Лучей", sf::Style::Default, sf::ContextSettings(32));
    main_window.setVerticalSyncEnabled(true);

    main_window.setActive(true);


    // --- Main Loop --- //

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (main_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...

        main_window.display();
    }

    return 0;
}