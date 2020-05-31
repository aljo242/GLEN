#include "BasicCam.h"

using namespace GLEN;


BasicCam::BasicCam(glm::vec3 position)
		:
	Front{glm::vec3(0.0f, 0.0f, -1.0f)},
	Up{glm::vec3(0.0f, 1.0f, 0.0f)},
	MovementSpeed{CamConstants::speed},
	Zoom{CamConstants::zoom},
	MouseSensitivity{CamConstants::sensitivity}
	{
		Position = position;
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		updateCameraVectors();
	}

void BasicCam::updateCameraVectors()
{
	// update "front" vector
	glm::vec3 front;
	front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
	front.y = std::sin(glm::radians(Pitch));
	front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up	  = glm::normalize(glm::cross(Right, Front));

}


void BasicCam::ProcessKeyboard(CameraMovement direction, const float deltaTime)
{
	const float velocity = MovementSpeed * deltaTime;
	switch (direction)
	{
	case CameraMovement::FORWARD:
		Position += Front * velocity;
		break;
	case CameraMovement::BACKWARD:
		Position -= Front * velocity;
		break;
	case CameraMovement::RIGHT:
		Position += Right * velocity;
		break;
	case CameraMovement::LEFT:
		Position -= Right * velocity;
		break;

	}
}

void GLEN::BasicCam::ProcessMouseMovement(const float xOffset, const float yOffset, const GLboolean constrainPitch)
{
	Yaw += xOffset * MouseSensitivity;
	Pitch -= yOffset * MouseSensitivity;

	if (constrainPitch)
	{
		if(Pitch > 89.0f )
		{
			Pitch = 89.0f;
		}
		else if(Pitch < -89.0f )
		{
			Pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void GLEN::BasicCam::ProcessMouseScroll(const float yOffset)
{
	Zoom -= static_cast<float>(yOffset);
	if (Zoom < 1.0f)
	{
		Zoom = 1.0f;
	}
	if (Zoom > 45.0f)
	{
		Zoom = 45.0f;
	}
}	

