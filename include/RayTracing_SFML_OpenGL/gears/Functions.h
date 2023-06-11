#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <iostream>


namespace gears
{
	// --- ImGui

	inline namespace literals
	{
		char const* operator"" _C(const char8_t* str, std::size_t);
	}

	ImVec2 operator+(const ImVec2& left, const ImVec2& right);
	ImVec2 operator-(const ImVec2& left, const ImVec2& right);
	bool operator==(const ImVec2& left, const ImVec2& right);
	bool operator!=(const ImVec2& left, const ImVec2& right);

	bool block();                           // ImGUI::Sameline(); && return true;
	void SelectableColor(ImU32 color);
	float get_button_width(const std::string& text, ImGuiStyle& style);

	// --- SFML

	extern bool keyHit[];
	extern bool ignore_input;

	int mouse_down(const sf::Mouse::Button& B);
	int key_down(const sf::Keyboard::Key& B);
	int key_hit(const sf::Keyboard::Key& key);

	// --- OpenGL

	struct Position
	{
		float x = 0.f, y = 0.f;
	};

	struct Color
	{
		float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
	};

	struct Vertex
	{
		Position pos;
		Color color;
	};

} // namespace gears
