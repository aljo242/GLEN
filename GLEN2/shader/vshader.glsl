#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	ourColor = vec3(0.8f, 0.6f, 0.42f);
	TexCoord = aTexCoord;

	// TODO move inverse calculation to CPU since it is expensive
	Normal = mat3(transpose(inverse(model))) * aNormal;  
;
}