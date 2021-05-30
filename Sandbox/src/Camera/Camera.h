#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <Hazel.h>

class Camera
{
public:
	Camera(glm::vec3 position = {0.0f, 0.0f, 1.0f}, glm::vec3 dir  , glm::vec3 up)
	{

	}

	void OnUpdate()
	{

		static const float speed = 1.0f;

		if (Hazel::Input::IsKeyPressed(Hazel::Key::W))
		{
			Position += (Forward * speed);
		}

		if (Hazel::Input::IsKeyPressed(Hazel::Key::S))
		{
			Position -= (Forward * speed);
		}

		if (Hazel::Input::IsKeyPressed(Hazel::Key::A))
		{
			Position += (Right * speed);
		}

		if (Hazel::Input::IsKeyPressed(Hazel::Key::B))
		{
			Position += (Right * speed);
		}
	}


	glm::mat4x4 GetViewMatrix()
	{
		return glm::lookAt(Position, (Forward - Position), { .0f, .1f, .0f });
	}
	
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
};
