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
	void SelectableColor(ImU32 color)
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

	bool mouseDown[GLFW_MOUSE_BUTTON_LAST] = { 0 };
	bool keyDown[GLFW_KEY_LAST] = { 0 };
	bool keyHit[GLFW_KEY_LAST] = { 0 };

	bool ignore_input = false;

	bool global_flags[2] = { 0 }; 
		// 0 - Window Resized
		// 1 - x

	bool mouse_down(int B)
	{
		if (ignore_input)
			return false;
		return mouseDown[B];
	}
	bool key_down(int B)
	{
		if (ignore_input)
			return false;
		return keyDown[B];
	}
	bool key_hit(int key)
	{
		if (!keyHit[key]) {
			if (keyDown[key]) {
				keyHit[key] = true;
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
		case GLFW_PRESS:   keyDown[key] = true;                       break;
		case GLFW_RELEASE: keyDown[key] = false; keyHit[key] = false; break;
		default:                                                      break;
		}
	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:   mouseDown[button] = true;  break;
		case GLFW_RELEASE: mouseDown[button] = false; break;
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
