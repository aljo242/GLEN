#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include "BasicCam.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GLEN
{
	void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

	class Window
	{
	public:
		Window(const std::string name, const int width, const int height);
		void DoFrame();
		BasicCam camera;

	private:
		GLFWwindow* window;
		void processInput();
		void processMouse();
		void processKeys();

		std::string m_name;
		int m_width;
		int m_height;

		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		const glm::vec3 cameraUp;

		float deltaTime {0.0f};
		float lastFrame {0.0f};


		float pitch {0.0f};
		float yaw {-90.0f};
		float roll {0.0f};

		float lastX {0.0f};
		float lastY {0.0f};
	};	
}
