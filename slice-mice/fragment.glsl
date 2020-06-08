#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lampColor;
uniform vec3 lightPos;

void main() {
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lampColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lampColor;

    vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}
