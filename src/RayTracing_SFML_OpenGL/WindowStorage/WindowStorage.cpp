#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>
#include <vector>

WindowStorage::WindowStorage(std::wstring window_title)
{
    window_.create(sf::VideoMode(1920u, 1080u), window_title, sf::Style::Default, sf::ContextSettings(32));
    // window_.setVerticalSyncEnabled(true);
    // window_.setFramerateLimit(60);
    
    if (!font.loadFromFile("cyrillic.ttf"))
    {
        std::cout << "Can't load font!" << std::endl;
    }
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
    // --- Ray Tracing

    glm::vec2 size{ window_.getSize().x, window_.getSize().y };

    window_.setActive(true);

    glBegin(GL_POINTS);
    
    float col_r, col_g;
    float pos_x, pos_y;

    int x, y;
    for (y = 0; y < size.y; ++y)
    {
        for (x = 0; x < size.x; ++x)
        {
            col_r = x / size.x;
            col_g = y / size.y;

            pos_x = 2.f * x / size.x - 1.f;
            pos_y = 2.f * y / size.y - 1.f;

            glColor3f(col_r, col_g, 0.f);
            glVertex2f(pos_x, pos_y);
        }
    }

    glEnd();

    window_.setActive(false);

    // --- FPS

    window_.pushGLStates();

    sf::Text fps_txt{std::to_string(ImGui::GetIO().Framerate), font, 20};
    fps_txt.setFillColor(sf::Color::White);
    window_.draw(fps_txt);

    window_.popGLStates();
}
