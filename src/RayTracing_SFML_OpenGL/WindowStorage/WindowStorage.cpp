#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(const std::string& window_title)
{
    int glfw_initiated = glfwInit();
    if (glfw_initiated != GLFW_TRUE)
    {
        printf("GLFW initialization failed");
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window_ = glfwCreateWindow(1920, 1080, window_title.c_str(), nullptr, nullptr);
    
    if (window_ == NULL)
    {
        printf("Failed to create GLFW window\n");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
    }
    
    gears::global_flags[0] = true;

    glfwSetKeyCallback(window_, gears::key_callback);
    glfwSetMouseButtonCallback(window_, gears::mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_, gears::framebuffer_size_callback);

    lastTime = glfwGetTime();
    deltaTime = 0;
}


void WindowStorage::close_window()
{
    glfwSetWindowShouldClose(window_, GL_TRUE);
}
void WindowStorage::shutdown()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}
void WindowStorage::pollEvents()
{
    glfwPollEvents();

    if (gears::global_flags[0] == true)
    {
        glfwGetFramebufferSize(window_, &screen_size.x, &screen_size.y);

        screen_ratio = (float)screen_size.x / (float)screen_size.y;

        glViewport(0, 0, screen_size.x, screen_size.y);

        m_ImageHorizontalIter.resize(screen_size.x);
        m_ImageVerticalIter.resize(screen_size.y);
        for (int i = 0; i < screen_size.x; i++)
            m_ImageHorizontalIter[i] = i;
        for (int i = 0; i < screen_size.y; i++)
            m_ImageVerticalIter[i] = i;
    }

    double nowTime = glfwGetTime();
    double deltaTime = nowTime - lastTime;
    lastTime = nowTime;

    // sf::Event event;
    // while (window_.pollEvent(event))
    // {
    //     ImGui::SFML::ProcessEvent(event);
    // 
    //     switch (event.type) {
    //     case sf::Event::Closed:
    //         running_ = false;
    //         break;
    //     case sf::Event::Resized: {
    //         window_.setActive(true);
    //         glViewport(0, 0, event.size.width, event.size.height);
    //         window_.setActive(false);
    // 
    //         window_.pushGLStates();
    //         sf::FloatRect visibleArea(0.f, 0.f,
    //             static_cast<float>(event.size.width),
    //             static_cast<float>(event.size.height));
    //         window_.setView(sf::View(visibleArea));
    //         window_.popGLStates();
    // 
    //         screen_size = window_.getSize();
    //         screen_ratio = (float)screen_size.x / (float)screen_size.y; }
    //         break;
    //     case sf::Event::KeyReleased:
    //         gears::keyHit[event.key.code] = false;
    //         break;
    //     default:
    //         break;
    //     }
    // }
}
bool WindowStorage::isRunning()
{
    return !glfwWindowShouldClose(window_);
}

void WindowStorage::ImGui_init()
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}
void WindowStorage::ImGui_update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void WindowStorage::ImGui_render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void WindowStorage::ImGui_shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool WindowStorage::window_has_focus()
{
    return glfwGetWindowAttrib(window_, GLFW_FOCUSED);
}

void WindowStorage::window_cls()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
void WindowStorage::window_flip()
{
    glfwSwapBuffers(window_);
}

glm::ivec2 WindowStorage::get_view_area()
{
    return screen_size;
}
double WindowStorage::get_frame_elapsed_time()
{
    return deltaTime * 1000;
}

void WindowStorage::render_view()
{
    // --- Ray Tracing

    glm::vec2 size{ get_view_area() };
    
    auto rotator = tracer::rotate_matrix(camera.angles.x, camera.angles.y);
    float focus_distance = 1.f / std::tanf(vFOV_half);
    
    glBegin(GL_POINTS);

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

                    // glColor3f(color.r, color.g, color.b);
                    // glVertex3f(pos_x, -pos_y, 0);
                }
            );
        }
    );
    
    glEnd();
}

tracer::Camera& WindowStorage::get_camera()
{
    return camera;
}

void WindowStorage::set_cursor_position(glm::vec2 position)
{
    glfwSetCursorPos(window_, position.x, position.y);
}
void WindowStorage::hide_mouse()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void WindowStorage::show_mouse()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::ivec2 WindowStorage::get_mouse_pos()
{
    glm::dvec2 pos = { 0, 0 };

    glfwGetCursorPos(window_, &pos.x, &pos.y);

    return { floor(pos.x), floor(pos.y) };
}

void WindowStorage::process_inputs()
{
    float delta_speed = float(moving_speed * get_frame_elapsed_time());

    if (gears::key_down(GLFW_KEY_SPACE))
        camera.move_up(delta_speed);
    if (gears::key_down(GLFW_KEY_LEFT_CONTROL))
        camera.move_down(delta_speed);
    if (gears::key_down(GLFW_KEY_W))
        camera.move_forward(delta_speed);
    if (gears::key_down(GLFW_KEY_S))
        camera.move_backward(delta_speed);
    if (gears::key_down(GLFW_KEY_D))
        camera.move_right(delta_speed);
    if (gears::key_down(GLFW_KEY_A))
        camera.move_left(delta_speed);
}
