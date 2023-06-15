#include <RayTracing_OpenGL/Tracer/Objects.h>
#include <RayTracing_OpenGL/Tracer/Tracer.h>


namespace tracer
{
    void Camera::rotate(float x, float y)
    {
        angles += gears::Angles{x, y};
    }

	void Camera::move_forward(float a)
	{
		gears::LookAt ray_direction = glm::vec3{ 0.f, 1.f, 0.f } * tracer::rotate_matrix(angles.x, angles.y);

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_backward(float a)
	{
		move_forward(-a);
	}
	void Camera::move_right(float a)
	{
		gears::LookAt ray_direction = glm::vec3{ 1.f, 0.f, 0.f } * tracer::rotate_matrix(angles.x, angles.y);

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_left(float a)
	{
		move_right(-a);
	}
	void Camera::move_up(float a)
	{
		gears::LookAt ray_direction = glm::vec3{ 0.f, 0.f, 1.f } * tracer::rotate_matrix(angles.x, angles.y);

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_down(float a)
	{
		move_up(-a);
	}

} // namespace tracer
