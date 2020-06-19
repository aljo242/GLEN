#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Log.h"
#include "Timer.h"
#include "BasicCam.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "KeyState.h"
#include "MouseState.h"

namespace GLEN{

struct InputState
{
	KeyState	keyState;
	MouseState	mouseState;
};

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

class Window
{
public:
	Window(const std::string name, const int width, const int height);
	void Run();
	BasicCam camera;
	Timer m_timer;

private:
	GLFWwindow* window;
	InputState processInput();
	MouseState processMouse();
	KeyState processKeys();

	std::string m_name;
	int m_width;
	int m_height;

	float lastX {0.0f};
	float lastY {0.0f};
};

} // namespace GLEN
