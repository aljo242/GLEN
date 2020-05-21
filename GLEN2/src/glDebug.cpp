#include "glDebug.h"

void APIENTRY GLEN::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	GLEN_ERROR("-------------------------------------");
	GLEN_ERROR("Debug message ( {} ): ", id);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             GLEN_ERROR("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   GLEN_ERROR("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: GLEN_ERROR("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     GLEN_ERROR("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     GLEN_ERROR("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           GLEN_ERROR("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               GLEN_ERROR("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: GLEN_ERROR("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  GLEN_ERROR("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         GLEN_ERROR("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         GLEN_ERROR("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              GLEN_ERROR("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          GLEN_ERROR("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           GLEN_ERROR("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               GLEN_ERROR("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         GLEN_ERROR("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       GLEN_ERROR("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          GLEN_ERROR("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: GLEN_ERROR("Severity: notification"); break;
	}
}