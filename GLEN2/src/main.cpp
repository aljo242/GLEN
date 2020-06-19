#include "Log.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace GLEN;

//TODO have logging occur in DEBUG and RELEASE but not DIST

int main()
{
	constexpr int width{ 1280 };
	constexpr int height{ 720 };

#if defined(_DEBUG)
	auto logger = Log();
#endif

	Window myWindow("Learn OpenGL", width, height);

	myWindow.Run();

	glfwTerminate();
	return 0;
}