#include <RayTracing_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(const std::wstring& window_title)
{
    window_.create(sf::VideoMode(1000u, 1000u), window_title, sf::Style::Default, sf::ContextSettings(32));
    // window_.setVerticalSyncEnabled(true);
    // window_.setFramerateLimit(60);

    screen_size = window_.getSize();
    screen_ratio = (float)screen_size.x / (float)screen_size.y;

    m_ImageHorizontalIter.resize(screen_size.x);
    m_ImageVerticalIter.resize(screen_size.y);
    for (unsigned int i = 0; i < screen_size.x; i++)
        m_ImageHorizontalIter[i] = i;
    for (unsigned int i = 0; i < screen_size.y; i++)
        m_ImageVerticalIter[i] = i;

    frame_buffer_ = new gears::Vertex[screen_size.x * screen_size.y];

    if (!font_.loadFromFile("cyrillic.ttf"))
    {
        std::cout << "Can't load font!" << std::endl;
    }
}

WindowStorage::~WindowStorage()
{
    delete[] frame_buffer_;
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
            screen_ratio = (float)screen_size.x / (float)screen_size.y;

            m_ImageHorizontalIter.resize(screen_size.x);
            m_ImageVerticalIter.resize(screen_size.y);
            for (unsigned int i = 0; i < screen_size.x; i++)
                m_ImageHorizontalIter[i] = i;
            for (unsigned int i = 0; i < screen_size.y; i++)
                m_ImageVerticalIter[i] = i; 

            delete[] frame_buffer_;
            frame_buffer_ = new gears::Vertex[screen_size.x * screen_size.y];

            }
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
    tracer::rotate_sun();

    glm::vec2 size{ window_.getSize().x, window_.getSize().y };
    
    auto rotator = tracer::rotate_matrix(camera.angles.x, camera.angles.y);
    float focus_distance = 1.f / std::tanf(vFOV_half);

    std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
        [this, size, &rotator, focus_distance](int y)
        {
            std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
            [this, y, size, &rotator, focus_distance](int x)
                {
                    float pos_x = 2.f * x / size.x - 1.f;
                    float pos_y = 2.f * y / size.y - 1.f;
            
                    gears::LookAt ray_direction{
                        pos_x * screen_ratio,
                        focus_distance,
                        -pos_y
                    };
            
                    ray_direction = ray_direction * rotator;
            
                    auto color = tracer::trace_ray(
                        tracer::Ray{
                            camera.origin,
                            glm::normalize(ray_direction)
                        }
                    );

                    this->frame_buffer_[y * (int)size.x + x].pos = { pos_x, pos_y };
                    this->frame_buffer_[y * (int)size.x + x].color = color;
                }
            );
        }
    );
    
    window_.setActive(true);

    glBegin(GL_POINTS);

    for (unsigned int i = 0; i < screen_size.x * screen_size.y; ++i)
    {
        auto point = frame_buffer_[i];
        glColor3f(point.color.r, point.color.g, point.color.b);
        glVertex2f(point.pos.x, -point.pos.y);
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
