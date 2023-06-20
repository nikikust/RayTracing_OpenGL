#include <RayTracing_OpenGL/Window/Window.h>


Window::Window(DataStorage& data_storage, const std::string window_title)
	:data_storage_(data_storage)
{
    create(window_title);
}
Window::~Window()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Window::close()
{
    glfwSetWindowShouldClose(window_, GL_TRUE);
}

void Window::on_resize()
{
    // --- Update render area size
    glfwGetFramebufferSize(window_, &data_storage_.screen_size.x, &data_storage_.screen_size.y);

    // --- Update screen ratio
    data_storage_.screen_ratio = (float)data_storage_.screen_size.x / (float)data_storage_.screen_size.y;

    // --- Resize viewport
    glViewport(0, 0, data_storage_.screen_size.x, data_storage_.screen_size.y);
}

bool Window::has_focus()
{
    return glfwGetWindowAttrib(window_, GLFW_FOCUSED);
}
bool Window::is_open()
{
    return !glfwWindowShouldClose(window_);
}

glm::ivec2 Window::get_mouse_pos()
{
    glm::dvec2 pos = { 0, 0 };

    glfwGetCursorPos(window_, &pos.x, &pos.y);

    return { floor(pos.x), floor(pos.y) };
}
void Window::set_cursor_position(glm::vec2 position)
{
    glfwSetCursorPos(window_, position.x, position.y);
}
void Window::hide_mouse()
{
    data_storage_.hide_mouse = true;
}
void Window::show_mouse()
{
    data_storage_.hide_mouse = false;
}

void Window::cls()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (data_storage_.hide_mouse)
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Window::flip()
{
    glfwSwapBuffers(window_);
}


void Window::create(const std::string window_title)
{
    init_GLFW();
    create_GLFW_window(window_title);
    init_GLAD();
    set_callbacks();

    on_resize();

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::init_GLFW()
{
    if (glfwInit() != GLFW_TRUE)
        std::cout << "GLFW initialization failed" << std::endl;
    else
        std::cout << "GLFW initialized" << std::endl;
}
void Window::create_GLFW_window(const std::string window_title)
{
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window_ = glfwCreateWindow(1920, 1080, window_title.c_str(), nullptr, nullptr);

    if (window_ == NULL)
        std::cout << "GLFW window creation failed" << std::endl;
    else
        std::cout << "GLFW window created" << std::endl;

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // VSync
}
void Window::init_GLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "GLAD initialization failed" << std::endl;
    else
        std::cout << "GLAD initialized" << std::endl;
}
void Window::set_callbacks()
{
    glfwSetKeyCallback(window_, gears::key_callback);
    glfwSetMouseButtonCallback(window_, gears::mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_, gears::framebuffer_size_callback);
}
