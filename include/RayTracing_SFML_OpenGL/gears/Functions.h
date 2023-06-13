#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "imgui_impl_opengl3.h"
#include <imgui_stdlib.h>
#include <imgui_internal.h>

//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
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

	// --- GLFW

	extern bool mouseDown[];
	extern bool keyDown[];
	extern bool keyHit[];

	extern bool ignore_input;

	extern bool global_flags[];

	int mouse_down(int B);
	int key_down(int B);
	int key_hit(int key);
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// --- OpenGL

	using Origin = glm::vec3;
	using LookAt = glm::vec3;
	using Angles = glm::vec2;
	using Color  = glm::vec4;

	struct Vertex
	{
		Origin pos;
		Color color;
	};

} // namespace gears
