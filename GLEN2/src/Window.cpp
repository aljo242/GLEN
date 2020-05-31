#include <filesystem>

#include "Window.h"
#include "Shader.h"
#include "glDebug.h"



#include "Defines.h"

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
	cameraPos{glm::vec3(0.0f, 0.0f, 3.0f)},
	cameraFront{glm::vec3(0.0f, 0.0f, -1.0f)},
	cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)},
	camera{glm::vec3(0.0f, 0.0f, 3.0f)}
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

	float vertices[]{
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

	unsigned int VAO, VBO;
	//std::vector<unsigned int> EBOs(1);
	constexpr int triangleSize{ 9 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(static_cast<int>(EBOs.size()), EBOs.data());

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	// color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texcoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//std::vector<float> borderColor { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());

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

	glm::mat4 view{ glm::lookAt(cameraPos,
								cameraPos + cameraFront,
								cameraUp)
	};

	constexpr float radius{ 10.0f };

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
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

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle{ 20.0f * i * static_cast<float>(glfwGetTime()) };
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources when done
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, EBOs.data());
}

void Window::processInput()
{
	float currentFrame	{static_cast<float>(glfwGetTime())};
	deltaTime			= currentFrame - lastFrame;
	lastFrame			= currentFrame;

	// probe for keys
	processKeys();

	// get cursor pos
	processMouse();
}

void GLEN::frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLEN::Window::processMouse()
{
	double xpos{0.0};
	double ypos{0.0};
	glfwGetCursorPos(window, &xpos, &ypos);

	float xOffset {static_cast<float>(xpos) - lastX};
	float yOffset {static_cast<float>(ypos) - lastY};
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
		camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);

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
