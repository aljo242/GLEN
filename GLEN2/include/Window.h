#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Log.h"

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
	};
}
