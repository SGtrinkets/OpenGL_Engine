#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>


// Plugins for OpenGL Mathematics
// Allows us to use mathematical transformations into our engine for any object
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

struct Texture {
	unsigned int id;
	string type;
	aiString path; // we store the path of the texture to compare it with other textures
};

#endif