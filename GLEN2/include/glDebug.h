#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"

namespace GLEN
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam);
}