

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader
{
public:
	unsigned int ID;

	//Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retreive the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertexShader, fragmentShader;
		int success;
		char infoLog[512];

		// vertex Shader

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// attaches the shader source code to the shader object then compiles it
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		//print compile errors if any
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}

		// fragment Shader

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}

		// shader program
		ID = glCreateProgram();

		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	//void use();
	void use() {
		glUseProgram(ID);
	}
	//void setBool(const std::string& name, bool value) const;
	void setBool(const std::string& name, bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	//void setInt(const std::string& name, int value) const;
	void setInt(const std::string& name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	//void setFloat(const std::string& name, float value) const;
	void setFloat(const std::string& name, float value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
};
#endif

