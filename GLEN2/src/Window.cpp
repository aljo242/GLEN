#include <filesystem>

#include "Window.h"
#include "glDebug.h"
#include "Timer.h"
#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"

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
	Shader lightCube("shader\\lightSourceV.glsl", "shader\\lightSourceF.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
	
	std::vector<GLfloat> vertices{
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f, 
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};
	

	// world space positions of our cubes
	std::vector<glm::vec3> cubePositions {
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

	std::vector<GLint> bufferList {3, 2, 3};

	// TODO encapsulate further
	VertexBuffer<GLfloat> vBuffer2(bufferList, vertices);
	vBuffer2.GenArrays(1);
	vBuffer2.GenBuffers(1);
	vBuffer2.Bind();
	vBuffer2.BufferStatic();
	vBuffer2.PushVert(0); vBuffer2.PushVert(1);  vBuffer2.PushVert(2);

	// TODO add this functionality to VertexBuffer
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer2.GetVBO());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0u);

	stbi_set_flip_vertically_on_load(true);
	std::filesystem::path path{ "res\\tex\\container.jpg" };
	TextureDescriptor descriptor;
	Texture tex1(path, descriptor);

	stbi_set_flip_vertically_on_load(true);
	path = "res\\tex\\awesomeface.png";
	descriptor.format = GL_RGBA;
	Texture tex2(path, descriptor);


	// wireframe render
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShader.Bind();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	glm::mat4 model{ glm::mat4(1.0f) };
	glm::mat4 projection{ glm::perspective(glm::radians(70.0f), static_cast<float>(m_width / m_height), 0.1f, 100.0f) };
	glm::mat4 view{camera.GetViewMatrix()};

	constexpr float radius{ 10.0f };
	constexpr bool trackFPS {true};

	const glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // white light 
	glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
	glm::vec3 result {lightColor * toyColor};

	const glm::vec3 lightSourcePos(1.2f, 1.0f, 2.0f);




	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		m_timer.update(trackFPS);

		processInput();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const float camX {static_cast<float>(std::sin(glfwGetTime()))};
		const float camY {static_cast<float>(std::cos(.002 * glfwGetTime()))};

		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(m_width / m_height), 0.1f, 1000.0f);
		view = camera.GetViewMatrix();

		size_t index {cubePositions.size() - 1};


		// DRAW
		ourShader.Bind();


		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setFloat("toggle", mixValue);

		ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("lightPos", glm::vec3(1.2f, camX * radius, -10.0f));
		ourShader.setVec3("viewPos", camera.GetPosition());

		//ourShader.setFloat("camX", camX);
		//ourShader.setFloat("camY", camY);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1.GetID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2.GetID());
		vBuffer2.BindArray();

		for (unsigned int i = 0; i < cubePositions.size() - 1; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle{ 20.0f * i * static_cast<float>(glfwGetTime()) };
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0,  36);
		}



		lightCube.Bind();
		lightCube.setMat4("projection", projection);
		lightCube.setMat4("view", view);

		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, camX * radius, -10.0f));
		float angle{ 20.0f * index * static_cast<float>(glfwGetTime()) };
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightCube.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0,  36);
		




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
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);
	
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