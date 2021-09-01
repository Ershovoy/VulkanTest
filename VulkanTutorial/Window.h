#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(int width, int height, std::string name);
	~Window();

	bool createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	bool shouldClose() { return !glfwWindowShouldClose(_pWindow); }

	int getWidth() const { return _width; }
	int getHeight() const { return _height; }

	GLFWwindow* getWindow() const { return _pWindow; }

private:
	GLFWwindow* _pWindow;

	int _width;
	int _height;
	
	std::string _windowName;
};

