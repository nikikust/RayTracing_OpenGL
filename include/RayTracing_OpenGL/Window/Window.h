#pragma once
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Window
{
public:
	Window(DataStorage& data_storage);
	Window() = delete;
	Window(Window&) = delete;
	Window(Window&&) = delete;

	// --- //

	void create();
	void close();

	void poll_events();

	void on_resize();

	void cls();
	void flip();

private:
	void init_GLFW();
	void create_GLFW_window();
	void init_GLAD();
	void set_callbacks();


	DataStorage& data_storage_;

};
