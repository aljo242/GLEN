#version 460 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float toggle;
uniform float camX;
uniform float camY;

void main()
{
	vec3 mixer = .5 * vec3(camX, camY, 1.0);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * -1.0f, TexCoord.y)), toggle) * vec4(mixer * ourColor, 1.0f);
}