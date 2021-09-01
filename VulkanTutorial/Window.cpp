#include "Window.h"

Window::Window(int width, int height, std::string name) : _width{ width }, _height{ height }, _windowName{ name }
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_pWindow = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(_pWindow);

	glfwTerminate();
}

bool Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, _pWindow, nullptr, surface) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}
