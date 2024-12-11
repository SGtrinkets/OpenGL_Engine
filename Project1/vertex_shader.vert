#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
layout (location = 1) in vec3 aColor;// color has attribute position 1
layout (location = 2) in vec2 aTexCoord;
out vec3 ourPos;
out vec3 ourColor; // specifies a color output to the fragment shader
out vec2 TexCoord;

//uniform float hOffset = -0.5f;


void main()
{
	ourPos = aPos;
	gl_Position = vec4(ourPos.x, ourPos.y, ourPos.z,1.0); // we give a vec3 a vec4's constructor
	ourColor = aColor; // set ourColor to input color from the vertex data
	TexCoord = aTexCoord;
};