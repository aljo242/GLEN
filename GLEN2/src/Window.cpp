#include <filesystem>

#include "Window.h"
#include "glDebug.h"
#include "Timer.h"
#include "Buffers.h"
#include "Shader.h"



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace GLEN;

constexpr char shaderPath[]{ "shader\\" };
float mixValue{ 0.0f };

Window::Window(const std::string name, const int width, const int height)
	:
	window{nullptr},
	m_name{name},
	m_width{width},
	m_height{height},
	camera{glm::vec3(0.0f, 0.0f, 3.0f)},
	m_timer()

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// TODO
	// add exception throw
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		GLEN_CRITICAL("Failed to initialize GLAD");
	}
}

void Window::DoFrame()
{
	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shader\\vshader.glsl", "shader\\fshader.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------

	std::vector<GLfloat> vertices{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::vector<GLint> bufferList {3, 2};

	VertexBuffer<GLfloat, GL_FLOAT> vBuffer2(bufferList, vertices);
	vBuffer2.GenArrays(1);
	vBuffer2.GenBuffers(1);
	vBuffer2.Bind();
	vBuffer2.BufferStatic();
	vBuffer2.PushVert(0); vBuffer2.PushVert(1); 

	// load some texture
	unsigned int texture1{ 0 };
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // map to:       x, y, z

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // map to:       x, y, z

	stbi_set_flip_vertically_on_load(true);

	int width{ 0 };
	int height{ 0 };
	int nrChannels{ 0 };
	std::filesystem::path path{ "res\\tex\\container.jpg" };
	unsigned char* data{ stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0) };
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		GLEN_CRITICAL("ERROR::TEXTURE::FAILED_LOAD");
	}
	stbi_image_free(data);

	unsigned int texture2{ 0 };
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // map to:       x, y, z

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // map to:       x, y, z

	stbi_set_flip_vertically_on_load(true);

	path = "res\\tex\\awesomeface.png";
	data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		GLEN_CRITICAL("ERROR::TEXTURE::FAILED_LOAD");
	}
	stbi_image_free(data);

	// wireframe render
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShader.Bind();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	glm::mat4 model{ glm::mat4(1.0f) };
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 view{ glm::mat4(1.0f) };
	glm::mat4 projection{ glm::perspective(glm::radians(70.0f), static_cast<float>(m_width / m_height), 0.1f, 100.0f) };

	//glm::vec3 cameraPos						{ glm::vec3(0.0f, 0.0f, 3.0f) };
	//constexpr glm::vec3 cameraTarget		{ glm::vec3(0.0f, 0.0f, 0.0f) };
	//const glm::vec3 cameraDirection			{ glm::normalize(cameraPos - cameraTarget)};
	//constexpr glm::vec3 up					{ glm::vec3(0.0f, 1.0f, 0.0f) };
	//const glm::vec3 cameraRight				{ glm::normalize(glm::cross(up, cameraDirection)) };
	//const glm::vec3 cameraUp				{ glm::cross(cameraDirection, cameraUp) };

	glm::mat4 view{camera.GetViewMatrix()};

	constexpr float radius{ 10.0f };
	constexpr bool trackFPS {false};

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		m_timer.update(trackFPS);

		processInput();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float camX = std::sin(glfwGetTime()) * radius;
		float camY = std::cos(.002 * glfwGetTime()) * radius;
		//cameraPos = glm::vec3(camX, 0.0f, camY);
		view = camera.GetViewMatrix();

		ourShader.Bind();
		//view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(m_width / m_height), 0.1f, 100.0f);
		// DRAW
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		ourShader.setFloat("toggle", mixValue);

		ourShader.setFloat("camX", camX);
		ourShader.setFloat("camY", camY);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		vBuffer2.BindArray();
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle{ 20.0f * i * static_cast<float>(glfwGetTime()) };
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0,  36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources when done

	//glDeleteBuffers(1, EBOs.data());
}

void Window::processInput()
{

	// probe for keys
	processKeys();

	// get cursor pos
	processMouse();
}


void GLEN::Window::processMouse()
{
	double xpos{0.0};
	double ypos{0.0};
	glfwGetCursorPos(window, &xpos, &ypos);

	const float xOffset {static_cast<float>(xpos) - lastX};
	const float yOffset {static_cast<float>(ypos) - lastY};
	lastX = xpos;
	lastY = ypos;
	
	constexpr GLboolean constrainPitch {true};
	camera.ProcessMouseMovement(xOffset, yOffset, constrainPitch);
}

void GLEN::Window::processKeys()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::FORWARD, m_timer.getDelta());
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::BACKWARD, m_timer.getDelta());

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::LEFT, m_timer.getDelta());

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::RIGHT, m_timer.getDelta());
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 1.0f)
		{
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f)
		{
			mixValue = 0.0f;
		}
	}
}

void GLEN::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}