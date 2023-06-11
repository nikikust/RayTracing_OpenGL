#include <RayTracing_SFML_OpenGL/gears/Functions.h>


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
	float get_button_width(std::string text, ImGuiStyle& style)
	{
		return ImGui::CalcTextSize(text.c_str()).x + style.FramePadding.x * 2 + style.ItemSpacing.x;
	}

	// --- SFML

	bool keyHit[sf::Keyboard::KeyCount] = { 0 };
	bool ignore_input = false;

	int mouse_down(const sf::Mouse::Button& B)
	{
		if (ignore_input)
			return false;
		return sf::Mouse::isButtonPressed(B);
	}
	int key_down(const sf::Keyboard::Key& B)
	{
		if (ignore_input)
			return false;
		return sf::Keyboard::isKeyPressed(B);
	}
	int key_hit(const sf::Keyboard::Key& key)
	{
		if (!keyHit[key]) {
			if (sf::Keyboard::isKeyPressed(key)) {
				keyHit[key] = true;
				if (!ignore_input)
					return true;
			}
		}
		return false;
	}
} // namespace gears
