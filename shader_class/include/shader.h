// Create a shader class that reads shaders from disk

#ifndef SHADER_H
#define SHADER_H

#include "graphics.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
	public:
		unsigned int ID; // program ID of the shader program
		Shader(const char* vertexPath, const char* fragmentPath); // constructor that reads and builds the shader
		void use(); // activate this shader

		// Functions to set uniform values
		void setBool(const std::string &name, bool value);
		void setInt(const std::string &name, int value);
		void setFloat(const std::string &name, float value);
		void set4Float(const std::string &name, float r,float b,float g,float a);
};	

#endif
