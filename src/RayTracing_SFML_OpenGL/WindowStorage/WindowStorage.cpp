#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(std::wstring window_title)
{
    window_.create(sf::VideoMode(400u, 400u), window_title, sf::Style::Default, sf::ContextSettings(32));
    window_.setVerticalSyncEnabled(true);
}


void WindowStorage::shutdown()
{
    running_ = false;
}
void WindowStorage::pollEvents()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);

        switch (event.type) {
        case sf::Event::Closed:
            running_ = false;
            break;
        case sf::Event::Resized: 
            glViewport(0, 0, event.size.width, event.size.height);
            break;
        case sf::Event::KeyReleased:
            keyHit[event.key.code] = false;
            break;
        default:
            break;
        }
    }
}
bool WindowStorage::isRunning()
{
    return running_;
}

void WindowStorage::ImGui_init()
{
    if (ImGui::SFML::Init(window_) == false)
        return;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    deltaClock_.restart();
}
void WindowStorage::ImGui_update()
{
    ImGui::SFML::Update(window_, deltaClock_.restart());
}
void WindowStorage::ImGui_render()
{
    ImGui::SFML::Render(window_);
}
void WindowStorage::ImGui_shutdown()
{
    ImGui::SFML::Shutdown();
}

bool WindowStorage::window_has_focus()
{
    return window_.hasFocus();
}

void WindowStorage::window_cls()
{
    window_.setActive(true);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    window_.setActive(false);
}
void WindowStorage::window_flip()
{
    window_.display();
}

void WindowStorage::render_view()
{
    window_.setActive(true);
    
    glBegin(GL_LINES);
     glVertex2f(-1.f, -1.f);
     glVertex2f( 1.f,  1.f);
    glEnd();
    
    window_.setActive(false);
}
