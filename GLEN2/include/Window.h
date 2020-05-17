#pragma once

#include "GLEN_PCH.h"
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
	};
}
