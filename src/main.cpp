#include "graphics.h"
#include "triangle_shaders.h"
#include <iostream>


// TODO: Clean up code and try the exercies at the end of this chapter: 
// https://learnopengl.com/Getting-started/Hello-Triangle

// Modern opengl requires you to define a vertex and fragement shader of your own. There is no default shader.
// A shader is a program that runs on a GPU core.

// Vertices for rendering a single triangle (that is why there are 3).
// Vertex values must be normalized device coordinates (between -1.0 and 1.0)
// Note all z values are 0, so this renders a 2d triangle.
// These coordinates will go through a viewpoint transform to end up as screen-space coordinates
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

// Draw a rectangle with two triangles
// You only need 4 vertices for a rectangle and the order to draw them in. This is what Element Buffer Objects do.
float bad_rect_vertices[] = {
	// First triangle
	0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	// Second triangle
	0.5f,-0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f
};

float better_rect_vertices[] = {
	0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f
};

unsigned int indices[] = {
	0,1,3, // first triangle
	1,2,3  // second triangle
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

	// Use wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	// Set back to normal mode
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Create VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);


	// Bind a VAO to use it to store the VBO attributes
	glBindVertexArray(VAO);

	// Need to declare memory on the GPU to store the vertices (vertex buffer objects)
	unsigned int VBO;
	glGenBuffers(1,&VBO); // This gives us 1 buffer, and the ID of the buffer is stored in VBO

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER,VBO); // GL_ARRAY_BUFFER is the vertex buffer object type

	// Binding makes all calls to GL_ARRAY_BUFFER go to the VBO.
	// Copy the data over to the GPU.
	glBufferData(GL_ARRAY_BUFFER,sizeof(better_rect_vertices),better_rect_vertices,GL_STATIC_DRAW); // GL_STATIC_DRAW - the data is set only once and used many times. GL_STATIC_DRAW will place the data in GPU memory that has fast reads but slow writes. GL_DYNAMIC_DRAW focuses on fast writes.

	// Create EBO for rectangle
	unsigned int EBO;
	glGenBuffers(1,&EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	// Need to create a vertex and fragment shader to start processing the data we just copied over. The shader must be written in the shader language GLSL (OpenGL Shading Language).
	// See shader in triangle_shaders.h file

	// Compile the shader. OpenGL compiles it dynamically at run-time. We need to create a shader object.
	unsigned vertexShader;
	// In this case it is a vertex shader since we are drawing a triangle.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach source code to shader object.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Shader compile error checking
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Next we need to create the fragment shader. Source is in triangle_shaders.h.
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	// Check for compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Now we need to link both shaders into a shader program
	// And activate the program when rendering objects

	// Create a program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for errors in linking program
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Tell opengl to use the program
	glUseProgram(shaderProgram);

	// Delete shader objects after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// We still need to tell OpenGL how to interpret the vertex data and how it should connect the data to the vertex shader attributes

	// Each point in the vertex is 4 bytes, and there are 3 points, so each vertex is 12 bytes
	// The vertex data is tightly packed x0,y0,z0,x1,y1,z1...
	// The first value in the data is at the beginning of the buffer

	// The 0 is from location 0 in the shader code. The 3 specifies vec3. The boolean is asking if we want the data to be normalized.
	// 3*sizeof(float) is the stride of the verticies
	// The last value is the offset for the beginning of the data.
	// Since the data is at the beginning, the offset is 0.
	// Note, it is doing this for the VBO we bound earlier
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float),(void*)0);
	// 0 is the attribute location
	glEnableVertexAttribArray(0);

	// Switching between all this every time you want to switch buffers is cumbersome.
	// Luckily there is a way in OpenGL to save the state for the bind and just switch buffer binds using Vertex Array Objects.
	// So, the attributes are saved in the VAO, and whenever we want to draw the object we simply bind that VAO
	// Core OpenGL requires that we use VAOs
	// This code needs to be before the VBO



	// Bind EBO buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);


	// Render loop
	while(!glfwWindowShouldClose(window))
	{
		// Poll input
		processInput(window);

		// Rendering commands
		// ...
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Triangle
		//glDrawArrays(GL_TRIANGLES,0,3); // 0 index of vertex array we want to draw, 3 is how many vertices our object is.
		// Draw rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

		// Check and call events and swap the frame buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glfwTerminate();

	return 0;
}
