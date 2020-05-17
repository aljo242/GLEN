#include "GLEN_PCH.h"
#include "Window.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace GLEN;

constexpr char shaderPath[]{ "shader\\" };
float mixValue {0.0f};

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
	Shader ourShader("shader\\vshader.glsl", "shader\\fshader.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
	std::vector<float> triangle{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	};

	std::vector<unsigned int> indices{
		0, 1, 3,
		1, 2, 3
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
	std::vector<unsigned int> VBOs(1);
	std::vector<unsigned int> VAOs(1);
	std::vector<unsigned int> EBOs(1);
	constexpr int triangleSize{ 9 };

	glGenVertexArrays(static_cast<int>(VAOs.size()), VAOs.data());
	glGenBuffers(static_cast<int>(VBOs.size()), VBOs.data());
	glGenBuffers(static_cast<int>(EBOs.size()), EBOs.data());

	for (int i{ 0 }; i < VBOs.size(); ++i)
	{
		glBindVertexArray(VAOs[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, static_cast<int>(triangle.size() * sizeof(float)), triangle.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);
		// pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		// color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	//std::vector<float> borderColor { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());

	// load some texture
	unsigned int texture1{ 0 };
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // map to:       x, y, z

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // map to:       x, y, z

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // map to:       x, y, z

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // tex coord are s, t, r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // map to:       x, y, z

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

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.GetID(), "texture1"), 0);
	ourShader.setInt("texture2", 1);


	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		processInput();

		float xValue{ (sin(static_cast<float>(glfwGetTime())) / 2.0f) };
		float yValue{ (sin(2.0f * static_cast<float>(glfwGetTime())) / 2.0f) };
		float zValue{ (sin(0.5f * static_cast<float>(glfwGetTime())) / 2.0f) };

        glClearColor(xValue + 0.5f, 0.3f, zValue + 0.5f, yValue + 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		int transformLoc {glGetUniformLocation(ourShader.GetID(), "transform")};
		int toggleLocation{ glGetUniformLocation(ourShader.GetID(), "toggle") };

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// DRAW
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glProgramUniform1f(ourShader.GetID(), toggleLocation, mixValue);



		//for (auto& VAO : VAOs)
		//{
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		//}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// de-allocate all resources when done
	glDeleteVertexArrays(1, VAOs.data());
	glDeleteBuffers(1, VBOs.data());
	glDeleteBuffers(1, EBOs.data());
}

void Window::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if(mixValue >= 1.0f)
		{	
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if(mixValue <= 0.0f)
		{	
			mixValue = 0.0f;
		}
	}
}

void GLEN::frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}