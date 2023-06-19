#pragma once
#include <RayTracing_OpenGL/Renderer/Tracer/Tracer.h>
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Renderer
{
public:
	Renderer(DataStorage& data_storage);
	Renderer() = delete;
	Renderer(Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	// --- //

	void update();
	void draw();

	void on_resize();

private:
	DataStorage& data_storage_;
	tracer::Tracer tracer_;

};
