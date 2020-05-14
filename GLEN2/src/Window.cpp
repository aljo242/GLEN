#include "GLEN_PCH.h"
#include "Window.h"

using namespace GLEN;

const char *vertexShaderSource {"#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0"};


const char *fragmentShaderSource1 {"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0" }; 

const char *fragmentShaderSource2 {"#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.1f, 0.7f, 0.5f);\n"
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

	std::vector<unsigned int> fragmentShaders(2);
	int counter {0};
	for (auto& shader : fragmentShaders)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		
		glShaderSource(shader, 1, &fragmentShaderSource2, NULL);
		if (counter == 1)
		{
			glShaderSource(shader, 1, &fragmentShaderSource1, NULL);
		}
		glCompileShader(shader);

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, logSize, NULL, infoLog);
			GLEN_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n");
			GLEN_ERROR(infoLog);
		}
	}
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShaders[1]);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
		GLEN_ERROR("ERROR::SHADER::PROGRAM::CREATION::FAILED\n");
		GLEN_ERROR(infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[1]);  

	    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> triangle  {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 

         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };

	// set up vertices
	// refactor to std::vector<float>
	/*
    float vertices[] = {
         0.9f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.9f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
	*/
	std::vector<unsigned int> VBOs(2);
	std::vector<unsigned int> VAOs(2);
	constexpr int triangleSize {9};

	glGenVertexArrays(VAOs.size(), VAOs.data());
	glGenBuffers(VBOs.size(), VBOs.data());

	for (int i {0}; i < VBOs.size(); ++i)
	{
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, triangleSize * sizeof(float), triangle.data() + i*triangleSize , GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	// wireframe render
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// rendering loop
	while(!glfwWindowShouldClose(window))
	{
		processInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW 
		glUseProgram(shaderProgram);

		for (int i {0}; i < VBOs.size(); ++i)
		{
			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// de-allocate all resources when done
	glDeleteVertexArrays(2, VAOs.data());
	glDeleteBuffers(1, VBOs.data());
	//glDeleteBuffers(1, &EBO);
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
