#pragma once
#include <execution>

#include <RayTracing_OpenGL/Renderer/Tracer/Tracer.h>
#include <RayTracing_OpenGL/DataStorage/DataStorage.h>


class Renderer
{
public:
	Renderer(DataStorage& data_storage);
	Renderer() = delete;
	Renderer(Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	~Renderer();

	// --- //

	void update();
	void draw();

	void on_resize(bool init = false);

private:
	//void prepare_shader();

	//void use_shader();

	void prepare_buffers();
	void prepare_shaders();

	gears::Color trace_ray(const tracer::Ray& ray);


	DataStorage& data_storage_;
	
	GLuint shader_program;         //
	GLuint id_VAO;                 // for shader
	GLuint id_VBO;                 //
	GLuint id_UBO;                 //
	GLfloat* dotPositions;         // 
	GLfloat* dotColors;            //
	GLintptr size_of_dotPositions; // for VBO
	GLintptr size_of_dotColors;    // 

	std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;
};
