#include <RayTracing_SFML_OpenGL/Tracer/Objects.h>


namespace tracer
{
    void Camera::rotate(float x, float y)
    {
        angles += gears::Angles{x, y};
    }

	void Camera::move_forward(float a)
	{
		gears::LookAt ray_direction{std::sinf(angles.x) * std::cosf(angles.y), std::cosf(angles.x) * std::cosf(angles.y), std::sinf(angles.y)};

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_backward(float a)
	{
		move_forward(-a);
	}
	void Camera::move_right(float a)
	{
		gears::LookAt ray_direction{std::sinf(angles.x + glm::half_pi<float>()) * std::cosf(angles.y), std::cosf(angles.x + glm::half_pi<float>()) * std::cosf(angles.y), std::sinf(angles.y)};

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_left(float a)
	{
		move_right(-a);
	}
	void Camera::move_up(float a)
	{
		gears::LookAt ray_direction{std::sinf(angles.x) * std::cosf(angles.y + glm::half_pi<float>()), std::cosf(angles.x) * std::cosf(angles.y + glm::half_pi<float>()), std::sinf(angles.y + glm::half_pi<float>())};

		origin += glm::normalize(ray_direction) * a;
	}
	void Camera::move_down(float a)
	{
		move_up(-a);
	}

} // namespace tracer
