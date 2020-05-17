#pragma once
#include <GLEN_PCH.h>

namespace GLEN
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		unsigned int GetID() const noexcept { return ID; }
		// use/activate the shader
		void use();

		// utility uniform functions
		void setBool(const std::string& name, const bool value) const;
		void setInt(const std::string& name, const int value) const;
		void setFloat(const std::string& name, const float value) const;

	private:
		unsigned int  ID;
	};
}
