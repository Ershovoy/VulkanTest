#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, char* name);
	~Window();

	bool createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	bool shouldClose() { return !glfwWindowShouldClose(_pWindow); }

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	GLFWwindow* getWindow() { return _pWindow; }

private:
	GLFWwindow* _pWindow;
	int _width;
	int _height;
	char* _windowName;
};

