#pragma once

#include "GLEN_PCH.h"
#include "Shader.h"
#include "Log.h"

/*
notes

change animation to focus on existing kernel
utilizing -> FEATURING

"exp

swap 2nd bullet on "Develop" to platforms
"investigate new hardware and software platforms"
"study" acceleration of various app domains

*/

using namespace GLEN;

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open the file
		vShaderFile.open(vertexPath.c_str());
		fShaderFile.open(fragmentPath.c_str());
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		// read the file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close the files
		vShaderFile.close();
		fShaderFile.close();
		// convert to strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		GLEN_CRITICAL("ERROR::SHADER::FILE::NOT::SUCCESSFULLY::READ");
	}

	const char* vShaderCode{ vertexCode.c_str() };
	const char* fShaderCode{ fragmentCode.c_str() };

	// 2. compile the shaders
	unsigned int vertex{ 0 };
	unsigned int fragment{ 0 };
	int success{ 0 };
	char infoLog[GLlogSize];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compilation errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, GLlogSize, NULL, infoLog);
		GLEN_CRITICAL("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
		GLEN_CRITICAL(infoLog);
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compilation errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, GLlogSize, NULL, infoLog);
		GLEN_CRITICAL("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
		GLEN_CRITICAL(infoLog);
	}

	// shader program time
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// error logging
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, GLlogSize, NULL, infoLog);
		GLEN_CRITICAL("ERROR::SHADER::PROGRAM::LINKING_FAILED");
		GLEN_CRITICAL(infoLog);
	}

	// delete shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}