// TODO FIX


#version 460 core

// fragmentshader

// material data container
struct Material {
	vec3 ambient;
	sampler2D emission;
	vec3 diffuse;
	sampler2D diffuseTex;
	vec3 specular;
	sampler2D specularTex;

	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform float toggle;
//uniform float camX;
//uniform float camY;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
		// calc ambient lighting
	vec3 ambient = light.ambient * .5 * (vec3(texture(material.diffuseTex, TexCoord)) + vec3(texture(material.emission, TexCoord)));

	// calc diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuseTex, TexCoord));



	// calc specular lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = specularStrength *pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specularTex, TexCoord));

	vec3 phongLight = ambient + diffuse + specular;
	FragColor = vec4(phongLight, 1.0);
}