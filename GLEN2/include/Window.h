#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Log.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GLEN
{
	void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height);

	class Window
	{
	public:
		Window(const std::string name, const int width, const int height);
		void DoFrame();

	private:
		GLFWwindow* window;
		void processInput();
		std::string m_name;
		int m_width;
		int m_height;

		glm::vec3 cameraPos;
		const glm::vec3 cameraFront;
		const glm::vec3 cameraUp;

		float deltaTime {0.0f};
		float lastFrame {0.0f};
	};
}
