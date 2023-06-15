#include <RayTracing_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(std::wstring window_title)
    : stp_(THREADPOOL_SIZE)
{
    window_.create(sf::VideoMode(1920u, 1080u), window_title, sf::Style::Default, sf::ContextSettings(32));
    // window_.setVerticalSyncEnabled(true);
    // window_.setFramerateLimit(60);
    
    if (!font_.loadFromFile("cyrillic.ttf"))
    {
        std::cout << "Can't load font!" << std::endl;
    }
}
WindowStorage::~WindowStorage()
{
    stp_.Join();
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

    int step = (int)ceil(size.y / (float)THREADPOOL_SIZE);


    std::vector<std::future<void>> results;
    results.reserve(THREADPOOL_SIZE);

    for (int i = 0; i < THREADPOOL_SIZE; ++i)
    {
        glm::vec2 start{0, i * step};
        glm::vec2 stop{size.x, (i + 1) * step};

        sf::RenderWindow& window = window_;
        auto trace_task = [this, size, start, stop, &window]() { trace_rays(start, stop, size, window); };

        results.emplace_back(gears::Async(stp_, trace_task));
    }

    for (auto& f : results) {
        f.wait();
    }

    // --- FPS

    window_.pushGLStates();

    sf::Text fps_txt{std::to_string(ImGui::GetIO().Framerate), font_, 20};
    fps_txt.setFillColor(sf::Color::White);
    window_.draw(fps_txt);

    window_.popGLStates();
}

void WindowStorage::trace_rays(const glm::vec2& start, const glm::vec2& stop, const glm::vec2& size, sf::RenderWindow& window) const
{
    if (stop.x < start.x || stop.y < start.y)
        return;

    std::array<gears::Vertex, 3000> line;
    // line.reserve((size_t)(stop.x - start.x + 2)); // reserve place for line
    
    gears::Vertex pix;

    for (int y = (int)start.y; y < stop.y && y < size.y; ++y)
    {
        // --- Calculate pixel line

        for (int x = (int)start.x; x < stop.x && x < size.x; ++x)
        {
            pix.color.r = x / size.x;
            pix.color.g = y / size.y;
            pix.color.b = 0.f;

            pix.pos.x = 2.f * x / size.x - 1.f;
            pix.pos.y = 2.f * y / size.y - 1.f;

            line[x] = pix;
        }
        // --- Submit pixel line

        {
            std::unique_lock lock(gl_mutex_);

            window.setActive(true);

            glBegin(GL_POINTS);

            for (int x = (int)start.x; x < stop.x && x < size.x; ++x)
            {
                glColor3f(line[x].color.r, line[x].color.g, line[x].color.b);
                glVertex2f(line[x].pos.x, line[x].pos.y);
            }

            glEnd();

            window.setActive(false);
        }

        // --- Prepare for next line
    }
}
