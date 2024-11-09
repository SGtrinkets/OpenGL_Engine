#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

using namespace std;

// An iteration of the render loop is more commonly called a frame

const unsigned int SCREEN_HEIGHT = 600;
const unsigned int SCREEN_WIDTH = 800;

// used to callback the viewport dimensions if a user wants to resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// checks to see whether the escape key is being pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	

	// creates a values to store the vertexShader, then creates it
	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	
	//-----------------------------------------------------
	//Shader Time 

	Shader ourShader("vertex_shader.vert", "fragment_shader.frag");

	// First, create the Vertex Buffer Objects and Vertex Array Objects
	// Vertex Buffer Objects maange the memory created on the GPU to store vertex data
	// Vertex Array Objects works similarly but instead stores the following vertex attributes 
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(1, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(1, VBOs);
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

	
	

	




	
	// render loop or while the window has not been instructed to be closed
	while (!glfwWindowShouldClose(window)) {
		// checks for key presses every frame
		processInput(window);

		// clears the colorbuffer
		glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);




		// Activates the Shader program
		ourShader.use();
		
		// retrieves the running time in seconds
		//float timeValue = glfwGetTime();
		// we change the color dynamically between 0.0 and 1.0 with the sin function
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		// queries the location of the ourColor
		//int vertexColorLocation = glGetUniformLocation(Shader.shader, "ourColor");
		// sets the uniform value
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// draws the first triangle from the data in the first Vertex Array Object
		
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


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
	//glDeleteProgram(shaderProgram2);


	glfwTerminate();
	return 0;

}
