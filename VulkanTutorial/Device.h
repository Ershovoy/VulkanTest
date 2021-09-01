#pragma once

#include "Window.h"

#include <vector>
#include <map>
#include <set>
#include <stdexcept>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> instanceExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
	int graphicFamily;
	int	presentFamily;

	bool graphicFamilyHasValue = false;
	bool presentFamilyHasValue = false;

	bool isComplete() { return graphicFamilyHasValue && presentFamilyHasValue; }
};

class Device
{
public:
	Device(GLFWwindow* window);
	~Device();

	VkDevice getDevice() { return _device; };
private:
	VkInstance _instance;
	VkSurfaceKHR _surface;
	VkPhysicalDevice _physicalDevice;
	VkDevice _device;

	VkCommandPool _commandPool;

	VkQueue _graphicQueue;
	VkQueue _presentQueue;

	void createInstance();
	void createSurface(GLFWwindow* window);
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createCommandPool();

	// Helper function for picking right pysical device
	int ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice);
	bool isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);
};

