#include "GLEN_PCH.h"
#include "Window.h"

using namespace GLEN;

const char *vertexShaderSource {"#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0"};


const char *fragmentShaderSource {"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0" }; 

Window::Window(const std::string name, const int width, const int height)
	:
	window(nullptr),
	m_name(name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	// TODO
	// add exception throw
	if (window == nullptr)
	{
		GLEN_CRITICAL("Failed to create GLFW window");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// TODO
	// add exception throw
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		GLEN_CRITICAL("Failed to initialize GLAD");
	}
}

void Window::DoFrame()
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success {0};
	constexpr int logSize {512};
	char infoLog[logSize];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, logSize, NULL, infoLog);
		GLEN_ERROR("ERROR::SHADER::VERTEX::COMPILATION::FAILED\n");
		GLEN_ERROR(infoLog);
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetProgramiv(fragmentShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragmentShader, logSize, NULL, infoLog);
		GLEN_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n");
		GLEN_ERROR(infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
		GLEN_ERROR("ERROR::SHADER::PROGRAM::CREATION::FAILED\n");
		GLEN_ERROR(infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);  

	// set up vertices
	// refactor to std::vector<float>
	float vertices[] {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};


	unsigned int VBO {0};
	glGenBuffers(1, &VBO);

	unsigned int VAO {0};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// copies the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// you can unbidn the VAO afterwards so other VAO calls won't accidentaly modifyy VAO
	glBindVertexArray(0);

	// wireframe render
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// rendering loop
	while(!glfwWindowShouldClose(window))
	{
		processInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW 
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // re-bind
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// de-allocate all resources when done
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteProgram(shaderProgram);
}

void Window::processInput()
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


void GLEN::frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
{
		glViewport(0, 0, width, height);
}
