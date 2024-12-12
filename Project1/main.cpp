#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// An iteration of the render loop is more commonly called a frame

const unsigned int SCREEN_HEIGHT = 600;
const unsigned int SCREEN_WIDTH = 800;

float visibility = 0.5f;

// used to callback the viewport dimensions if a user wants to resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// checks to see whether the escape key is being pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		visibility += 0.001f;
		if (visibility >= 1.0f) {
			visibility = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		visibility -= 0.001f;
		if (visibility <= 0.0f) {
			visibility = 0.0f;
		}
	}

}



GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			break;
		}
		std::cout << error << "!" << file << "(" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei,
	const char* message, const void* userParam) 
{	
	// warnings and error codes that can be ignored
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "----------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source:WindowSystem";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source:ShaderCompiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source:ThirdParty"; break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source:Application"; break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source:Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type:Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type:DeprecatedBehaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type:UndefinedBehaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type:Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type:Performance"; break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type:Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type:PushGroup"; break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type:PopGroup"; break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type:Other"; break;
	}std::cout << std::endl;
	
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity:high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity:medium"; break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity:low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity:notification"; break;
	}std::cout << std::endl;
}



int main() {


	float vertices1[] = {
		// positions	  // colors
		0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
	};
	/*
	float vertices1[] = {
		-0.5f,-0.5f, 0.0f, //bottom left
		0.5f,-0.5f, 0.0f, // bottom right
		0.0f, 0.5f, 0.0f // top
	};
	// vertices to draw two triangles

	// Exercise 1: draw two triangles
	float vertices1[] = {
		//first triangle
		-0.5f,-0.5f, 0.0f, //bottom left
		0.0f,-0.5f, 0.0f, // bottom right
		0.0f, 0.5f, 0.0f, // top
	};
	*/
	float vertices2[] = {
		//second triangle
		0.0f,-0.5f, 0.0f, //bottom left
		0.5f,-0.5f, 0.0f, // bottom right
		0.5f, 0.5f, 0.0f // top
	};
	

	
	// we start from 0
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};


	float rectangleVertices[] = {
		// 0-2 positions // 3-5 colors    //6-7 texture coordinates 
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
	};

	

	// creates a values to store the vertexShader, then creates it
	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// allows for extensive debugging
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	
	//-----------------------------------------------------
	//Shader Time 

	Shader ourShader("vertex_shader.vert", "fragment_shader.frag");
	
	// First, create the Vertex Buffer Objects and Vertex Array Objects
	// Vertex Buffer Objects maange the memory created on the GPU to store vertex data
	// Vertex Array Objects works similarly but instead stores the following vertex attributes 
	unsigned int VBOs[2], VAOs[2], EBOs[2];

	
	// First rectangle with texture setup
	glGenVertexArrays(1, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(1, VBOs);
	glGenBuffers(1, EBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	/*
	// first triangle setup
	// --------------------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//std::cout << glGetError() << std::endl;


	//glBindBuffer(GL_VERTEX_ARRAY, VBOs);
	//glCheckError();


	// glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
	// second triangle setup
	// ---------------------
	/*
	glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glEnableVertexAttribArray(0);
	*/


	/*
	glGenVertexArrays(1, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(1, VBOs);
	// first triangle setup
	// --------------------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	*/





	
	
	//-----------------------------------------------------
	//Texture Time 

	stbi_set_flip_vertically_on_load(true);

	unsigned int texture, texture2;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // activates the texture unit first
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}


	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1); // activates the texture unit first
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	data = stbi_load("lighthouse.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	// frees up data from the texture
	stbi_image_free(data);

	
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);



	// render loop or while the window has not been instructed to be closed
	while (!glfwWindowShouldClose(window)) {
		// checks for key presses every frame
		processInput(window);

		// clears the colorbuffer
		glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		
		// Activates the Shader program
		//ourShader.use();
		
		// retrieves the running time in seconds
		//float timeValue = glfwGetTime();
		// we change the color dynamically between 0.0 and 1.0 with the sin function
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		// queries the location of the ourColor
		//int vertexColorLocation = glGetUniformLocation(Shader.shader, "ourColor");
		// sets the uniform value
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// draws the first triangle from the data in the first Vertex Array Object
		
		//glBindVertexArray(VAOs[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		

	
		glActiveTexture(GL_TEXTURE0); // activates the texture unit first
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1); // activates the texture unit first
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.setFloat("visibility", visibility);

		ourShader.use();
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//glUseProgram(shaderProgram2);
		//glBindVertexArray(VAOs[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(2, EBOs);
	//glDeleteProgram(shaderProgram2);


	glfwTerminate();
	return 0;

}


