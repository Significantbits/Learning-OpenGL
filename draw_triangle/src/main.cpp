#include "graphics.h"
#include "triangle_shaders.h"
#include "compile_shaders.h"
#include <iostream>
#include <math.h>


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
	0.0f, 0.5f, 0.0f,
	-0.9f, -0.5f, 0.0f,
	-0.6f, -0.5f, 0.0f,
	-0.75f, 0.5f, 0.0f,
};

// Lets now add color data
// We also need to change the vertex shader to take in color data
float tri_vertices1[] = {
	// Position			// Color
	-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f, // bottom right
	0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f, // bottom left
	0.0f, 0.5f, 0.0f,   0.0f,0.0f,1.0f  // top
};

float tri_vertices2[] = {
	-0.9f, -0.5f, 0.0f,
	-0.6f, -0.5f, 0.0f,
	-0.75f, 0.5f, 0.0f,
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

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&nrAttributes);
	std::cout << "Number of vertex attributes: " << nrAttributes << "\n";

	// Use wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	// Set back to normal mode
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Create VAOs
	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);


	// Bind a VAO to use it to store the VBO attributes
	glBindVertexArray(VAO1);

	// Need to declare memory on the GPU to store the vertices (vertex buffer objects)
	unsigned int VBO1;
	glGenBuffers(1,&VBO1); // This gives us 1 buffer, and the ID of the buffer is stored in VBO

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER,VBO1); // GL_ARRAY_BUFFER is the vertex buffer object type

	// Binding makes all calls to GL_ARRAY_BUFFER go to the VBO.
	// Copy the data over to the GPU.
	glBufferData(GL_ARRAY_BUFFER,sizeof(tri_vertices1),tri_vertices1,GL_STATIC_DRAW); // GL_STATIC_DRAW - the data is set only once and used many times. GL_STATIC_DRAW will place the data in GPU memory that has fast reads but slow writes. GL_DYNAMIC_DRAW focuses on fast writes.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float),(void*)0);
	// 0 is the attribute location
	glEnableVertexAttribArray(0);

	// Need to add another vertex attribute since we added color data to the vertices
	// 6 * sizeof(float) must be the size of 1 row of the array. 3 vertices, 3 colors.
	// 3 * sizeof(float) must define the row offset
	// Remember the GL_FALSE means are the values normalized
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6* sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Bind a VAO to use it to store the VBO attributes for second triangle
	glBindVertexArray(VAO2);

	// VBO for second triangle
	unsigned int VBO2;
	glGenBuffers(1,&VBO2); // This gives us 1 buffer, and the ID of the buffer is stored in VBO


	// Bind the buffer for second triangle
	glBindBuffer(GL_ARRAY_BUFFER,VBO2); // GL_ARRAY_BUFFER is the vertex buffer object type

	// Copy the data over to the GPU for second triangle
	glBufferData(GL_ARRAY_BUFFER,sizeof(tri_vertices2),tri_vertices2,GL_STATIC_DRAW); // GL_STATIC_DRAW - the data is set only once and used many times. GL_STATIC_DRAW will place the data in GPU memory that has fast reads but slow writes. GL_DYNAMIC_DRAW focuses on fast writes.

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


	// Create EBO for rectangle
	unsigned int EBO;
	glGenBuffers(1,&EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	// Need to create a vertex and fragment shader to start processing the data we just copied over. The shader must be written in the shader language GLSL (OpenGL Shading Language).
	// See shader in triangle_shaders.h file
	unsigned int vertexShader = compile_shaders(vertexCinShaderSource,0);
	unsigned int fragmentShader = compile_shaders(fragmentShaderNCSource,1);
	//unsigned int fragmentShaderRed = compile_shaders(fragmentShaderSourceRed,1);
	//unsigned int fragmentShaderYellow = compile_shaders(fragmentShaderSourceYellow,1);

	// Now we need to link both shaders into a shader program
	// And activate the program when rendering objects
	//unsigned int shaderProgramRed = link_shaders(2,vertexShader,fragmentShaderRed);
	//unsigned int shaderProgramYellow = link_shaders(2,vertexShader,fragmentShaderYellow);
	unsigned int shaderProgram = link_shaders(2,vertexShader,fragmentShader);


	// Tell opengl to use the program
	glUseProgram(shaderProgram);


	// Delete shader objects after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//glDeleteShader(fragmentShaderRed);
	//glDeleteShader(fragmentShaderYellow);


	// Switching between all this every time you want to switch buffers is cumbersome.
	// Luckily there is a way in OpenGL to save the state for the bind and just switch buffer binds using Vertex Array Objects.
	// So, the attributes are saved in the VAO, and whenever we want to draw the object we simply bind that VAO
	// Core OpenGL requires that we use VAOs
	// This code needs to be before the VBO



	// Bind EBO buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	// Bind a VAO to use it to store the VBO attributes for second triangle

	// UNIFORMS: here is where we set the uniform value. We need to do this after compiling and linking the shader program
	// This function also changes the color overtime
	// NOTE: YOU MUST CALL USEPROGRAM ON THE PROGRAM THAT HAS THE UNIFORM IN IT WHEN UPDATING THE VALUE
	// Uniforms aren't great if you want a color for each vertex. Then you would need to declare as many uniforms as there are vertices in the shape.
	// The answer to this is to add color data to the vertices array. This way we can store the information in a VBO. Then store the VBO's attributes in a VAO.
	float timevalue;
	float greenvalue;
	int vertexColorLocation;



	// Render loop
	while(!glfwWindowShouldClose(window))
	{
		// Poll input
		processInput(window);

		// Rendering commands
		// ...
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Function to change uniform value overtime
		timevalue = glfwGetTime();
		greenvalue = (sin(timevalue) / 2.0f)  + 0.5f;
		vertexColorLocation = glGetUniformLocation(shaderProgram,"Color");

		// Set the uniform value
		glUniform4f(vertexColorLocation,0.0f,greenvalue,0.0f,1.0f);


		// Draw Triangle
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES,0,3); // 0 index of vertex array we want to draw, 3 is how many vertices our object is.

		// Tell opengl to use the program
		glUseProgram(shaderProgram);

		// For second triangle
		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES,0,3); // 0 index of vertex array we want to draw, 3 is how many vertices our object is.
		// Draw rectangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

		// Check and call events and swap the frame buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up
	glfwTerminate();

	return 0;
}
