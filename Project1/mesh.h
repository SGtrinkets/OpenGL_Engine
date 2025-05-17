#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

// Plugins for OpenGL Mathematics
// Allows us to use mathematical transformations into our engine for any object
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include the vector header to resolve the error
#include <vector>
#include <string>

using namespace glm;
using namespace std;

struct Vertex {
	vec3 position; //position coordinates 
	vec3 normal; // normal vector 
	vec2 texCoords; // texture coordinates
};

struct Texture {
	unsigned int id;
	string type; // can be diffuse, specular, normal, etc.
};

class Mesh {
	public:
		// mesh data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture>	textures;
		unsigned int VAO;

		// constructor
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
			vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			setupMesh();
		}

		void Draw(Shader& shader) {
			// bind appropriate textures
			unsigned int diffuseCount = 1;
			unsigned int specularCount = 1;
			unsigned int normalCount = 1;


			for (unsigned int i = 0; i < textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
				// retrieve texture number (diffuse or specular)
				string number;
				string name = textures[i].type;
				if (name == "texture_diffuse") {
					number = to_string(diffuseCount++);
				}
				else if (name == "texture_specular") {
					number = to_string(specularCount++); // we have a lot of specular lights, so we need to keep track of them
				}
				else if (name == "texture_normal") {
					number = to_string(normalCount++);
				}

				// Now set the sampler to the correct texture unit
				shader.setInt(("material." + name + number).c_str(), i);
				// and finally bind the texture
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}

			// sets active texture unit before binding
			glActiveTexture(GL_TEXTURE0);


			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		// render data
		unsigned int VBO, EBO;
		// render the mesh
		void setupMesh() {
			// create buffers/arrays
			glGenVertexArrays(1, &VAO); // Creates 1 Vertex Array Object 
			// stores the state of all the vertex attribute pointers (VBOs) and determines 
			// how the vertex data is stored in the GPU memory

			glGenBuffers(1, &VBO); // Creates 1 Vertex Buffer Object
			// stores vertex data (pos, colors, normals, texcoords, etc.) in GPU memory to 
			// reduce CPU-GPU data transfer AND improve performance

			glGenBuffers(1, &EBO); // Creates 1 Element Buffer Object
			// stores indices of vertices in GPU memory to reduce duplication of vertex data
			// (e.g. a vertex can be used in multiple triangles or cubes)

			// bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// fill buffer
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
				&vertices[0], GL_STATIC_DRAW);

			// now bind the Element Buffer Object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
				sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*)0);

			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, normal));

			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, texCoords));
			glBindVertexArray(0);
		}
};


#endif