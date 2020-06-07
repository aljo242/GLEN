#version 460 core

out vec4 FragColor;

in  vec3 ourColor;
in  vec3 Normal;
in  vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float toggle;
//uniform float camX;
//uniform float camY;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// calc ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// calc diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// calc specular lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = specularStrength *pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = spec * lightColor;

	vec3 phongLight = ambient + diffuse + specular;
	FragColor = vec4(phongLight * objectColor, 1.0);
}