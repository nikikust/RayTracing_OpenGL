#pragma once
#include <execution>
#include <random>

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
    void prepare_buffers();
    void prepare_shaders();

    void catch_errors(GLuint vertex_shader, GLuint fragment_shader, GLuint shader_program);

    // - Data
    DataStorage& data_storage_;
    
    // - Shader data
    GLuint shader_program;
    GLuint id_VAO;        
    GLuint id_VBO;
    GLuint id_UBO_spheres;
    GLuint id_UBO_materials;

    // - Randomizer
    std::random_device randomizer;
    std::mt19937 e2;
    std::uniform_real_distribution<> dist{0.0f, 1.0f};
};
