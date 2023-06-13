#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(const std::wstring& window_title)
{
    window_.create(sf::VideoMode(1000u, 1000u), window_title, sf::Style::Default, sf::ContextSettings(32));
    // window_.setVerticalSyncEnabled(true);
    // window_.setFramerateLimit(60);

    screen_size = window_.getSize();
    screen_ratio = (float)screen_size.x / (float)screen_size.y;

    if (!font_.loadFromFile("cyrillic.ttf"))
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
        case sf::Event::Resized: {
            window_.setActive(true);
            glViewport(0, 0, event.size.width, event.size.height);
            window_.setActive(false);

            window_.pushGLStates();
            sf::FloatRect visibleArea(0.f, 0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height));
            window_.setView(sf::View(visibleArea));
            window_.popGLStates();

            screen_size = window_.getSize();
            screen_ratio = (float)screen_size.x / (float)screen_size.y; }
            break;
        case sf::Event::KeyReleased:
            gears::keyHit[event.key.code] = false;
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
    time_elapsed_ = deltaClock_.restart();
    ImGui::SFML::Update(window_, time_elapsed_);
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

sf::Vector2u WindowStorage::get_view_area()
{
    return screen_size;
}
sf::Time WindowStorage::get_frame_elapsed_time()
{
    return time_elapsed_;
}

void WindowStorage::render_view()
{
    // --- Ray Tracing

    glm::vec2 size{ window_.getSize().x, window_.getSize().y };

    window_.setActive(true);
    
    glBegin(GL_POINTS);
    
    auto rotator = tracer::rotate_matrix(camera.angles.x, camera.angles.y);
    float focus_distance = 1.f / std::tanf(vFOV_half);
    
    for (int y = 0; y < size.y; ++y)
    {
        for (int x = 0; x < size.x; ++x)
        {
            float pos_x = 2.f * x / size.x - 1.f;
            float pos_y = 2.f * y / size.y - 1.f;
            
            gears::LookAt ray_direction{
                pos_x * screen_ratio,
                focus_distance,
                -pos_y
            };
            
            ray_direction = ray_direction * rotator;
    
            // gears::Angles angles = { camera.angles.x + pos_x * hFOV_half, 
            //                          camera.angles.y + pos_y * vFOV_half };
            // 
            // gears::LookAt ray_direction{
            //     std::sinf(angles.x) * std::cosf(angles.y), 
            //     std::cosf(angles.x) * std::cosf(angles.y), 
            //     std::sinf(angles.y)
            // };
            
            auto color = tracer::trace_ray(
                tracer::Ray{
                    camera.origin,
                    glm::normalize(ray_direction)
                }
            );
    
            // if (x == 0 && y%100 == 0 || x == size.x - 1 && y % 100 == 0)
            // {
            //     std::cout << std::setw( 6) << x
            //               << std::setw( 6) << y
            //               << std::setw(12) << ray_direction.x
            //               << std::setw(12) << ray_direction.y
            //               << std::setw(12) << ray_direction.z 
            //               << std::setw(12) << angles.x
            //               << std::setw(12) << angles.y
            //               << std::endl;
            // }
            // gears::Color color {0.3f, 0.6f, 1.0f, 1.0f};
    
            glColor3f(color.r, color.g, color.b);
            glVertex2f(pos_x, -pos_y);
        }
    }
    
    glEnd();

    window_.setActive(false);

    // --- FPS

    window_.pushGLStates();

    sf::Text fps_txt{std::to_string(ImGui::GetIO().Framerate), font_, 20};
    fps_txt.setFillColor(sf::Color::White);
    window_.draw(fps_txt);

    window_.popGLStates();
}

tracer::Camera& WindowStorage::get_camera()
{
    return camera;
}

void WindowStorage::set_cursor_position(sf::Vector2u position)
{
    sf::Mouse::setPosition({ (int)position.x, (int)position.y }, window_);
}
void WindowStorage::hide_mouse()
{
    window_.setMouseCursorVisible(false);
}
void WindowStorage::show_mouse()
{
    window_.setMouseCursorVisible(true);
}
