#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Plugins for OpenGL Mathematics
// Allows us to use mathematical transformations into our engine for any object
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "shader.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace glm;

// An iteration of the render loop is more commonly called a frame

const unsigned int SCREEN_HEIGHT = 600;
const unsigned int SCREEN_WIDTH = 800;

// mouse rotations (pitch is x, yaw is y)
// default value for yaw  is -90 so that it starts at the negative z-axis by default
//float g_yaw = -90.0f;
//float g_pitch = 0.0f;

// mouse positions recorded in last frame
float lastX = 400, lastY = 300;

bool firstMouse = true;


Camera camera(vec3(0.0f, 0.0f, 3.0f));

float visibility = 0.5f;

// Keeps track between current frame and last frame.
float deltaTime = 0.0f;
// Keeps track of time since last frame
float lastFrame = 0.0f;


// used to callback the viewport dimensions if a user wants to resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// xpos and ypos represent the current mouse positions respectively.
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xoffset = xpos - lastX; // x ranges from right to left
	float yoffset = lastY - ypos; //reversed  since y ranges from bottom to top

	// resets the last positions of the mouse to the current positions
	lastX = xpos;
	lastY = ypos;

	// adding the offset values to the rotation values
	camera.ProcessMouseMovement(xoffset, yoffset);

}

// Mouse scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	// checks to see whether the escape key is being pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	/*
	char key = '\0';
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
		key = 'W';
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
		key = 'S';
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		key = 'A';
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		key = 'D';
	}
	if (key != '\0')
		cout << "Key Pressed: " << key << endl;

	*/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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


	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f,0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		-0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f,0.0f,

		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,

		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f,-0.5f, 1.0f,1.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,

		-0.5f, 0.5f,-0.5f, 0.0f,1.0f,
		0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,0.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,1.0f
	};

	vec3 cubePositions[] = {
		 vec3(0.0f, 0.0f, 0.0f),
		 vec3(2.0f, 5.0f,-15.0f),
		 vec3(-1.5f,-2.2f,-2.5f),
		 vec3(-3.8f,-2.0f,-12.3f),
		 vec3(2.4f,-0.4f,-3.5f),
		 vec3(-1.7f, 3.0f,-7.5f),
		 vec3(1.3f,-2.0f,-2.5f),
		 vec3(1.5f, 2.0f,-2.5f),
		 vec3(1.5f, 0.2f,-1.5f),
		 vec3(-1.3f, 1.0f,-1.5f)
	};


	// we start from 0
	// used by element buffer objects to store reusable vertices for OpenGL to use
	// for certain shapes
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int indices2[] = {
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

	float rectangleVertices2[] = {
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

	// Register callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// renders the mouse invisible when the application is in focus
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertex_shader.vert", "fragment_shader.frag");
	//Shader ourShader2("vertex_shader.vert", "fragment_shader.frag");

	// First, create the Vertex Buffer Objects, Vertex Array Objects, and Element Buffer Objects
	// Vertex Buffer Objects maange the memory created on the GPU to store vertex data
	// Vertex Array Objects works similarly but instead stores the following vertex attributes 
	// Element Buffer Objects
	unsigned int VBOs[2], VAOs[2], EBOs[2];


	// First cube with texture setup
	glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);

	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		//(void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second cube with texture setup

	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		//(void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);




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
	//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	//ourShader2.use();
	//glUniform1i(glGetUniformLocation(ourShader2.ID, "texture1"), 0);
	//ourShader2.setInt("texture2", 1);



	// Camera Time



		// render loop or while the window has not been instructed to be closed
		while (!glfwWindowShouldClose(window)) {
			// checks for key presses every frame
			processInput(window);

			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// clears the colorbuffer
			glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



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



			// activates the texture unit first
			glActiveTexture(GL_TEXTURE0);
			// Binds the texture
			glBindTexture(GL_TEXTURE_2D, texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);


			// Activates the Shader program
			ourShader.use();

			
			//camera time
			unsigned int viewLoc, projectionLoc;

			//model matrix, rotated on the x axis
			//mat4 model = mat4(1.0f);
			//model = rotate(model, (float) glfwGetTime(), vec3(0.5f, 1.0f, 0.0f));

			
			mat4 view = camera.GetViewMatrix();

			
			// projection matrix
			mat4 projection = perspective(radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

			viewLoc = glGetUniformLocation(ourShader.ID, "view");
			//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			ourShader.setMat4("view", view);


			projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
			//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));
			ourShader.setMat4("projection", projection);
			

			// Binds the Vertex Array Object
			glBindVertexArray(VAOs[0]);

			for (unsigned int i = 0; i < 10; i++) {
				mat4 model = mat4(1.0f);
				model = translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// check and call events and swap the buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//cleanup(VAOs, VBOs, EBOs);
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(2, EBOs);

	glfwTerminate();
	return 0;

}


void cleanup(unsigned int* VAOs, unsigned int* VBOs, unsigned int* EBOs) {
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(2, EBOs);
}

