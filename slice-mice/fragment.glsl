#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec2 texc1 = TexCoord;

	vec4 texel1 = texture(texture1, texc1);
	vec2 texc2 = TexCoord;
	vec4 texel2 = texture(texture2, texc2);
	FragColor = mix(texel1, texel2, 0.05);
}