#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Read in the shader files and convert them to c_str
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vFile;
	std::ifstream fFile;

	vFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vFile.open(vertexPath);
		fFile.open(fragmentPath);

		std::stringstream vStream, fStream;
		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();

		vFile.close();
		fFile.close();

		vertexCode = vStream.str();
		fragmentCode = fStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";	
	}

	const char* vCode = vertexCode.c_str();
	const char* fCode = fragmentCode.c_str();


	// Compile shaders
	unsigned int vertex,fragment;
	int success;
	char infoLog[512];

	// Compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1,&vCode,NULL);
	glCompileShader(vertex);

	// Check for errors
	glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
	if(!success)
	{
		glGetShaderInfoLog(vertex,512,NULL,infoLog);	
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << "\n";
	}


	// Compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment,1,&fCode,NULL);
	glCompileShader(fragment);

	// Check for errors
	glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
	if(!success)
	{
		glGetShaderInfoLog(fragment,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << "\n";
	}

	// Link shader program
	ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);

	// Check for link errors
	glGetShaderiv(ID,GL_LINK_STATUS,&success);
	if(!success)
	{
		glGetShaderInfoLog(ID,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::LINK::FAILED\n" << infoLog << "\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name,bool value)
{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int) value);
}

void Shader::setInt(const std::string &name,int value)
{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const std::string &name,float value)
{
	glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::set4Float(const std::string &name, float r,float b,float g,float a)
{
	glUniform4f(glGetUniformLocation(ID,name.c_str()),r,b,g,a);
}
