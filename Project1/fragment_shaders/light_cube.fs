#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // camera position in world space

in vec3 FragPos;
in vec3 Normal;

float specularStrength = 0.65;

void main()
{
	//FragColor = vec4(lightColor * objectColor, 1.0);
	float ambientStrength = 0.11;
	vec3 ambient = ambientStrength * lightColor;

	// calculates the unit vector in the same direction as the original vector
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos- FragPos);

	// takes the max possible value of the diffuse value and then multiply it by the lightColor to get the max diffuse impact
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos- FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}