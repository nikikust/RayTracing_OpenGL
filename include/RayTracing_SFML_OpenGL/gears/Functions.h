#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <cstddef>


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
float get_button_width(std::string text, ImGuiStyle& style);

// --- SFML

extern bool keyHit[];
extern bool ignore_input;

int mouse_down(const sf::Mouse::Button& B);
int key_down(const sf::Keyboard::Key& B);
int key_hit(const sf::Keyboard::Key& key);