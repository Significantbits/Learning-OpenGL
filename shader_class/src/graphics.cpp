#include "graphics.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window,float* x_offset,float* y_offset)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
	if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		(*x_offset) -= 0.01f;
	}
	if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		(*x_offset) += 0.01f;
	}
	if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
	{
		(*y_offset) += 0.01f;
	}
	if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		(*y_offset) -= 0.01f;
	}
}
