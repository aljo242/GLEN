#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

namespace GLEN
{
	namespace CamConstants
	{
		constexpr float yaw			{-90.0f};
		constexpr float pitch		{0.0f};
		constexpr float speed		{2.5f};
		constexpr float sensitivity {0.05f};
		constexpr float zoom		{45.0f};
	}

	enum class CameraMovement
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class BasicCam
	{
	public:
		BasicCam(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));


		glm::mat4 GetViewMatrix() {return glm::lookAt(Position, Position + Front, Up);}

		void ProcessKeyboard(CameraMovement direction, const float deltaTime);
		void ProcessMouseMovement(const float xOffset, const float yOffset, const GLboolean constrainPitch = true);
		void ProcessMouseScroll(const float yOffset);

		
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up; 
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw   {CamConstants::yaw};
		float Pitch {CamConstants::pitch};

		float lastX {0.0f};
		float lastY {0.0f};

		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;


	private:

		void updateCameraVectors();
	};
}