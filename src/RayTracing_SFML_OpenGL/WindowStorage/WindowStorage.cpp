#include <RayTracing_SFML_OpenGL/WindowStorage/WindowStorage.h>


WindowStorage::WindowStorage(const std::string& window_title)
{
    init_GLFW();

    create_GLFW_window(window_title);
    
    init_GLAD();

    prepare_buffers();

    on_resize(true);

    prepare_shaders();

    prepare_callbacks();

    init_ImGui();

    lastTime = glfwGetTime();
    deltaTime = 0;
}
WindowStorage::~WindowStorage()
{
    delete[] OpenGL_data_.dotPositions;
    delete[] OpenGL_data_.dotColors;
}

// --- //

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
        on_resize();
        gears::global_flags[0] = 0;
    }

    double nowTime = glfwGetTime();
    deltaTime = nowTime - lastTime;
    lastTime = nowTime;
}
bool WindowStorage::isRunning()
{
    return !glfwWindowShouldClose(window_);
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

void WindowStorage::update()
{
    // --- Ray Tracing

    glm::vec2 size{ get_view_area() };
    
    auto rotator = tracer::rotate_matrix(camera.angles.x, camera.angles.y);
    float focus_distance = 1.f / std::tanf(vFOV_half);

    std::for_each(std::execution::par_unseq, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
        [this, size, &rotator, focus_distance](int y)
        {
            std::for_each(std::execution::par_unseq, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
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

                    this->OpenGL_data_.dotPositions[(y * (int)size.x + x) * 2 + 0] =  pos_x;
                    this->OpenGL_data_.dotPositions[(y * (int)size.x + x) * 2 + 1] = -pos_y;

                    this->OpenGL_data_.   dotColors[(y * (int)size.x + x) * 4 + 0] = color.r;
                    this->OpenGL_data_.   dotColors[(y * (int)size.x + x) * 4 + 1] = color.g;
                    this->OpenGL_data_.   dotColors[(y * (int)size.x + x) * 4 + 2] = color.b;
                    this->OpenGL_data_.   dotColors[(y * (int)size.x + x) * 4 + 3] = color.a;
                }
            );
        }
    );

    glBindBuffer(GL_ARRAY_BUFFER, OpenGL_data_.dotVBO);
    glBufferData(GL_ARRAY_BUFFER, OpenGL_data_.size_of_dotPositions + OpenGL_data_.size_of_dotColors, NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0,                                 OpenGL_data_.size_of_dotPositions, OpenGL_data_.dotPositions);
    glBufferSubData(GL_ARRAY_BUFFER, OpenGL_data_.size_of_dotPositions, OpenGL_data_.size_of_dotColors,    OpenGL_data_.dotColors);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WindowStorage::render_view()
{
    // Use shader program
    glUseProgram(OpenGL_data_.shader_program);

    // Bind VAO
    glBindVertexArray(OpenGL_data_.dotVAO);

    // Draw points
    glDrawArrays(GL_POINTS, 0, (GLsizei)(OpenGL_data_.size_of_dotPositions / sizeof(float) / 2));
}

// --- //

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
    float delta_speed = (float)
        (get_frame_elapsed_time() * moving_speed *
            (gears::key_down(GLFW_KEY_LEFT_SHIFT) ? 2.f : 1.f));

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

// --- Init methods

void WindowStorage::init_GLFW()
{
    if (glfwInit() != GLFW_TRUE)
        std::cout << "GLFW initialization failed" << std::endl;
    else
        std::cout << "GLFW initialized" << std::endl;
}
void WindowStorage::create_GLFW_window(const std::string& window_title)
{
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window_ = glfwCreateWindow(1980, 1080, window_title.c_str(), nullptr, nullptr);

    if (window_ == NULL)
        std::cout << "GLFW window creation failed" << std::endl;
    else
        std::cout << "GLFW window created" << std::endl;

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
}
void WindowStorage::init_GLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "GLAD initialization failed" << std::endl;
    else
        std::cout << "GLAD initialized" << std::endl;
}
void WindowStorage::prepare_buffers()
{
    glGenBuffers(1, &OpenGL_data_.dotVBO);
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL_data_.dotVBO);

    glGenVertexArrays(1, &OpenGL_data_.dotVAO);
    glBindVertexArray(OpenGL_data_.dotVAO);
}
void WindowStorage::prepare_shaders()
{
    // --- Compile shaders
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_source = gears::load_shader_text("shaders/shader.vert");

    const char* vs = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_source = gears::load_shader_text("shaders/shader.frag");

    const char* fs = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);

    // --- Create shader program
    OpenGL_data_.shader_program = glCreateProgram();
    glAttachShader(OpenGL_data_.shader_program, vertex_shader);
    glAttachShader(OpenGL_data_.shader_program, fragment_shader);
    glLinkProgram(OpenGL_data_.shader_program);

    // --- Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}
void WindowStorage::prepare_callbacks()
{
    glfwSetKeyCallback(window_, gears::key_callback);
    glfwSetMouseButtonCallback(window_, gears::mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_, gears::framebuffer_size_callback);
}
void WindowStorage::init_ImGui()
{
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

// --- Event methods

void WindowStorage::on_resize(bool init)
{
    // --- Update render area size
    glfwGetFramebufferSize(window_, &screen_size.x, &screen_size.y);

    // --- Update screen ratio
    screen_ratio = (float)screen_size.x / (float)screen_size.y;

    // --- Resize viewport
    glViewport(0, 0, screen_size.x, screen_size.y);

    // --- Update ray tracing boundaries
    m_ImageHorizontalIter.resize(screen_size.x);
    m_ImageVerticalIter.resize(screen_size.y);
    for (int i = 0; i < screen_size.x; i++)
        m_ImageHorizontalIter[i] = i;
    for (int i = 0; i < screen_size.y; i++)
        m_ImageVerticalIter[i] = i;

    // --- Realloc frame buffer
    if (!init)
    {
        delete[] OpenGL_data_.dotPositions;
        delete[] OpenGL_data_.dotColors;
    }

    OpenGL_data_.dotPositions = new float[screen_size.x * screen_size.y * 2];
    OpenGL_data_.dotColors    = new float[screen_size.x * screen_size.y * 4];

    OpenGL_data_.size_of_dotPositions = 2 * sizeof(float) * screen_size.x * screen_size.y;
    OpenGL_data_.size_of_dotColors    = 4 * sizeof(float) * screen_size.x * screen_size.y;
    
    // --- Recalculate OpenGL buffer pointers
    glBindBuffer(GL_ARRAY_BUFFER, OpenGL_data_.dotVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)(0));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(OpenGL_data_.size_of_dotPositions));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
