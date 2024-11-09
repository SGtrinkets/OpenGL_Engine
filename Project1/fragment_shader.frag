#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 ourPos;
void main()
{
	FragColor = vec4(ourColor, 1.0); // assigns the fragment shader's color to the color assigned in the vertexShader
}