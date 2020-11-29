#include "graphics.h"
#include "shader.h"
#include <iostream>


// Modern opengl requires you to define a vertex and fragement shader of your own. There is no default shader.
// A shader is a program that runs on a GPU core.

// Vertices for rendering a single triangle (that is why there are 3).
// Vertex values must be normalized device coordinates (between -1.0 and 1.0)
// Note all z values are 0, so this renders a 2d triangle.
// These coordinates will go through a viewpoint transform to end up as screen-space coordinates

// Lets now add color data
// We also need to change the vertex shader to take in color data
float tri_vertices_colors[] = {
	// Position			// Color
	-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f, // bottom right
	0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f, // bottom left
	0.0f, 0.5f, 0.0f,   0.0f,0.0f,1.0f  // top
};

float tri_vertices[] = {
	// Position			
	-0.5f, -0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

float inverted_tri_vertices[] = {
	// Position			
	-0.5f, 0.5f, 0.0f, 
	0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f
};

int main(int argc, char** argv)
{
	// Initialize glfw
	glfwInit();
	// Tell glfw we want to use opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	// Tell glfw we want to use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	// Make the window our current opengl context on the current thread
	glfwMakeContextCurrent(window);

	// GLAD manages function pointers for OpenGL, so we need to initalize it before working with OpenGL
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n";
		return -1;
	}

	// Tell OpenGL what we want the size of the viewport to be. Just make it the size of the window.
	// First two arguments set position
	glViewport(0,0,800,600);

	// Register viewport resize callback when window resizes - Implemented in graphics.h
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	// Create VAO to store VBO attributes and bind it
	unsigned int VAO;
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	// Create VBO to allocate and store vertex array data on GPU and bind it
	unsigned int VBO;
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	// Copy the data to the GPU
	glBufferData(GL_ARRAY_BUFFER,sizeof(inverted_tri_vertices),inverted_tri_vertices,GL_STATIC_DRAW);

	// Define vertex attributes to tell the shader how the data is laid out
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	// Create and compile shader
	Shader s("./shaders/move_vertex.vs","./shaders/uniform_fragment.fs");

	// Use shader program
	s.use();
	s.set4Float("Color",0.5f,0.0f,0.0f,1.0f);

	float x_offset = 0.0f;
	float y_offset = 0.0f;

	while(!glfwWindowShouldClose(window))
	{
		processInput(window,&x_offset,&y_offset);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		glDrawArrays(GL_TRIANGLES,0,3);
		s.setFloat("x_offset",x_offset);
		s.setFloat("y_offset",y_offset);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}	

	// clean up
	glfwTerminate();

	return 0;
}
