#include <RayTracing_OpenGL/Renderer/Renderer.h>


Renderer::Renderer(DataStorage& data_storage)
    : data_storage_(data_storage),
      e2(randomizer())
{
    prepare_buffers();
    prepare_shaders();

    on_resize(true);
}
Renderer::~Renderer()
{
    // delete[] dotPositions;
    // delete[] dotColors;
}

void Renderer::update()
{
    glUseProgram(shader_program);

    // - Materials
    GLint materials_amount_uniform = glGetUniformLocation(shader_program, "materials_amount");
    glUniform1i(materials_amount_uniform, (GLint)data_storage_.materials.size());

    // - Objects
    GLint spheres_amount_uniform = glGetUniformLocation(shader_program, "spheres_amount");
    glUniform1i(spheres_amount_uniform, (GLint)data_storage_.spheres.size());

    // - Scene info
    GLint sun_direction_uniform = glGetUniformLocation(shader_program, "sun_direction");
    glUniform3fv(sun_direction_uniform, 1, glm::value_ptr(data_storage_.sun_direction));

    GLint zenith_color_uniform = glGetUniformLocation(shader_program, "zenith_color");
    glUniform4fv(zenith_color_uniform, 1, glm::value_ptr(data_storage_.zenith_color));

    GLint horizon_color_uniform = glGetUniformLocation(shader_program, "horizon_color");
    glUniform4fv(horizon_color_uniform, 1, glm::value_ptr(data_storage_.horizon_color));
    
    // - Camera
    auto rotator = tracer::rotation_matrix(data_storage_.camera.angles.x, data_storage_.camera.angles.y);
    GLint camera_rotation_uniform = glGetUniformLocation(shader_program, "camera_rotation");
    glUniformMatrix3fv(camera_rotation_uniform, 1, GL_FALSE, glm::value_ptr(rotator));

    GLint camera_position_uniform = glGetUniformLocation(shader_program, "camera_position");
    glUniform3fv(camera_position_uniform, 1, glm::value_ptr(data_storage_.camera.origin));

    // - Randomizer
    GLint u_seed1_uniform = glGetUniformLocation(shader_program, "u_seed1");
    glUniform2f(u_seed1_uniform, (float)dist(e2) * 999.0f, (float)dist(e2) * 999.0f);

    GLint u_seed2_uniform = glGetUniformLocation(shader_program, "u_seed2");
    glUniform2f(u_seed2_uniform, (float)dist(e2) * 999.0f, (float)dist(e2) * 999.0f);
}

void Renderer::draw()
{
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::on_resize(bool init)
{
    glUseProgram(shader_program);

    // - Screen

    GLint resolution_uniform = glGetUniformLocation(shader_program, "resolution");
    glUniform2f(resolution_uniform, (float)data_storage_.screen_size.x, (float)data_storage_.screen_size.y);

    GLint screen_ratio_uniform = glGetUniformLocation(shader_program, "screen_ratio");
    glUniform1f(screen_ratio_uniform, data_storage_.screen_ratio);

    float focus_distance = 1.f / tanf(data_storage_.vFOV_half);
    GLint focus_distance_uniform = glGetUniformLocation(shader_program, "focus_distance");
    glUniform1f(focus_distance_uniform, focus_distance);
}


void Renderer::prepare_buffers()
{
    // Create a full-screen quad
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f
	};
	
	// --- VAO

	glGenVertexArrays(1, &id_VAO);
	glBindVertexArray(id_VAO);

	// --- VBO

	glGenBuffers(1, &id_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, id_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // --- UBO for Materials

    glGenBuffers(1, &id_UBO_materials);
    glBindBuffer(GL_UNIFORM_BUFFER, id_UBO_materials);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(tracer::Material) * 128, data_storage_.materials.data(), GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, id_UBO_materials);

	// --- UBO for Spheres

	glGenBuffers(1, &id_UBO_spheres);
	glBindBuffer(GL_UNIFORM_BUFFER, id_UBO_spheres);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(tracer::Sphere) * 128, data_storage_.spheres.data(), GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, id_UBO_spheres);
}
void Renderer::prepare_shaders()
{
    // --- Compile shaders
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_shader_source = gears::load_shader_text("shaders/vertex.glsl");

    const char* vs = vertex_shader_source.c_str();
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_shader_source = gears::load_shader_text("shaders/fragment.glsl");

    const char* fs = fragment_shader_source.c_str();
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);

    // --- Create shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // --- Catch errors

    catch_errors(vertex_shader, fragment_shader, shader_program);

    // --- Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
void Renderer::catch_errors(GLuint vertex_shader, GLuint fragment_shader, GLuint shader_program)
{
    GLint testval;
    bool abort = false;

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &testval);
    if (testval == GL_FALSE)
    {
        abort = true;
        char infolog[1024];
        glGetShaderInfoLog(vertex_shader, 1024, NULL, infolog);
        std::cout << "\nThe vertex shader failed to compile with the error:" << std::endl << infolog << std::endl;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &testval);
    if (testval == GL_FALSE)
    {
        abort = true;
        char infolog[1024];
        glGetShaderInfoLog(fragment_shader, 1024, NULL, infolog);
        std::cout << "\nThe fragment shader failed to compile with the error:" << std::endl << infolog << std::endl;
    }

    glGetProgramiv(shader_program, GL_LINK_STATUS, &testval);
    if (testval == GL_FALSE)
    {
        abort = true;
        char infolog[1024];
        glGetProgramInfoLog(shader_program, 1024, NULL, infolog);
        std::cout << "\nThe program failed to compile with the error:" << std::endl << infolog << std::endl;
    }

    if (abort)
    {
        std::cout << "Errors occured, cannot continue, aborting." << std::endl;
        exit(-1);
    }
    else
    {
        std::cout << "Shaders are compiled and linked successfully";
    }
}
