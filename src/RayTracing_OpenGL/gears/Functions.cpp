#include <RayTracing_OpenGL/gears/Functions.h>


namespace gears
{
    inline namespace literals
    {
        char const* operator"" _C(const char8_t* str, std::size_t)
        {
            return reinterpret_cast<const char*>(str);
        }
    }

    ImVec2 operator+(const ImVec2& left, const ImVec2& right)
    {
        return ImVec2(left.x + right.x, left.y + right.y);
    }
    ImVec2 operator-(const ImVec2& left, const ImVec2& right)
    {
        return ImVec2(left.x - right.x, left.y - right.y);
    }
    bool operator==(const ImVec2& left, const ImVec2& right)
    {
        return left.x == right.x && left.y == right.y;
    }
    bool operator!=(const ImVec2& left, const ImVec2& right)
    {
        return left.x != right.x || left.y != right.y;
    }

    bool block()
    {
        ImGui::SameLine();
        return true;
    }
    void selectable_color(ImU32 color)
    {
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, color);
    }
    float get_button_width(const std::string& text, ImGuiStyle& style)
    {
        return ImGui::CalcTextSize(text.c_str()).x + style.FramePadding.x * 2 + style.ItemSpacing.x;
    }

    // --- GLWF

    bool mouse_down_array[GLFW_MOUSE_BUTTON_LAST] = { 0 };
    bool key_down_array[GLFW_KEY_LAST] = { 0 };
    bool key_hit_array[GLFW_KEY_LAST] = { 0 };

    bool ignore_input = false;

    bool global_flags[2] = { 0 }; 
        // 0 - Window Resized
        // 1 - x

    bool mouse_down(int B)
    {
        if (ignore_input)
            return false;
        return mouse_down_array[B];
    }
    bool key_down(int B)
    {
        if (ignore_input)
            return false;
        return key_down_array[B];
    }
    bool key_hit(int key)
    {
        if (!key_hit_array[key]) {
            if (key_down_array[key]) {
                key_hit_array[key] = true;
                if (!ignore_input)
                    return true;
            }
        }
        return false;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:   key_down_array[key] = true;                       break;
        case GLFW_RELEASE: key_down_array[key] = false; key_hit_array[key] = false; break;
        default:                                                      break;
        }
    }
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:   mouse_down_array[button] = true;  break;
        case GLFW_RELEASE: mouse_down_array[button] = false; break;
        default:                                      break;
        }
    }
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        global_flags[0] = true;

        glViewport(0, 0, width, height);
    }

    // --- OpenGL

    std::string load_shader_text(const std::string& path)
    {
        std::ifstream in{ path };

        return { std::istreambuf_iterator<char>{ in }, std::istreambuf_iterator<char>{} };
    }

} // namespace gears
