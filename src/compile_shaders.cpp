#include "graphics.h"
#include <stdarg.h>
#include <iostream>

// Shader type is fragment or vertex
unsigned int compile_shaders(const char* shaderSource, int shader_type)
{
	unsigned int returnShader;
	int success;
	char infoLog[512];

	if(shader_type == 0)	
	{
		// Compile the shader. OpenGL compiles it dynamically at run-time. We need to create a shader object.
		// In this case it is a vertex shader since we are drawing a triangle.
		returnShader = glCreateShader(GL_VERTEX_SHADER);

		// Attach source code to shader object.
		glShaderSource(returnShader, 1, &shaderSource, NULL);
		glCompileShader(returnShader);

		// Shader compile error checking
		glGetShaderiv(returnShader, GL_COMPILE_STATUS, &success);

		if(!success)
		{
			glGetShaderInfoLog(returnShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
			exit(-1);
		}
	}
	else if(shader_type == 1)
	{
		// Next we need to create the fragment shader. Source is in triangle_shaders.h.
		returnShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(returnShader, 1, &shaderSource, NULL);
		glCompileShader(returnShader);
		
		// Check for compile errors
		glGetShaderiv(returnShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(returnShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
			exit(-1);
		}
	}
	else
	{
		std::cout << "ERROR: Invalid shader type\n";
		exit(-1);
	}

	return returnShader;
}

unsigned int link_shaders(int num,...)
{
	unsigned int returnProgram;
	returnProgram = glCreateProgram();

	va_list ap;
	va_start(ap,num);
	for(int i = 0 ; i < num ; i++)
	{
		glAttachShader(returnProgram, va_arg(ap, unsigned int));
	}	
	va_end(ap);
	glLinkProgram(returnProgram);

	int success;
	char infoLog[512];

	// Check for errors in linking program
	glGetProgramiv(returnProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(returnProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
		exit(-1);
	}

	return returnProgram;
}
