#include "Device.h"

Device::Device(GLFWwindow* window)
{
	createInstance();
	createSurface(window);
	pickPhysicalDevice();
	createLogicalDevice();
	createCommandPool();
}

Device::~Device()
{
	vkDestroyCommandPool(_device, _commandPool, nullptr);

	vkDestroyDevice(_device, nullptr);

	vkDestroySurfaceKHR(_instance, _surface, nullptr);

	vkDestroyInstance(_instance, nullptr);
}

void Device::createInstance()
{
	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	createInfo.pApplicationInfo = &appInfo;
	if (enableValidationLayers)
	{
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.enabledLayerCount = validationLayers.size();
	}
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();
	createInfo.enabledExtensionCount = instanceExtensions.size();

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create intance!");
	}
}

void Device::pickPhysicalDevice()
{
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		throw std::runtime_error("Failed to find GPU with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevices.data());

	std::map<int, VkPhysicalDevice> candidates;
	for (const VkPhysicalDevice& physicalDevice : physicalDevices)
	{
		if (isPhysicalDeviceSuitable(physicalDevice))
		{
			int score = ratePhysicalDeviceSuitability(physicalDevice);
			candidates.insert(std::make_pair(score, physicalDevice));
		}
	}

	if (candidates.rbegin()->first > 0) {
		_physicalDevice = candidates.rbegin()->second;
	}
	else {
		throw std::runtime_error("Failed to find suitable GPU!");
	}
}

void Device::createSurface(GLFWwindow* window)
{
	if (glfwCreateWindowSurface(_instance, window, nullptr, &_surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}

void Device::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledExtensionCount = deviceExtensions.size();

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(_device, indices.graphicFamily, 0, &_graphicQueue);
	vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);
}

void Device::createCommandPool()
{
	QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);

	VkCommandPoolCreateInfo createInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	createInfo.queueFamilyIndex = indices.graphicFamily;

	if (vkCreateCommandPool(_device, &createInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool!");
	}
}


int Device::ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice)
{
	int score = 0;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 100;
	}
	else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		score += 50;
	}

	return score;
}

bool Device::isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice)
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	bool isExtensionSuppoted = checkDeviceExtensionSupport(physicalDevice);

	bool isSwapChainSuppotRightFormat = false;
	if (isExtensionSuppoted)
	{
		SwapChainSupportDetails swapChainDetails = querySwapChainSupport(physicalDevice);
		isSwapChainSuppotRightFormat = swapChainDetails.formats.empty() && swapChainDetails.presentModes.empty();
	}

	return indices.isComplete() && isExtensionSuppoted && isSwapChainSuppotRightFormat;
}

QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice physicalDevice)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilyCount; ++i)
	{
		if (queueFamilies[i].queueFlags == VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicFamily = i;
			indices.graphicFamilyHasValue = true;
		}

		VkBool32 isPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, _surface, &isPresentSupport);

		if (isPresentSupport == true)
		{
			indices.presentFamily = i;
			indices.presentFamilyHasValue = true;
		}

		if (indices.isComplete()) {
			break;
		}
	}
	return indices;
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice physicalDevice)
{
	SwapChainSupportDetails swapChainDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, _surface, &swapChainDetails.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, nullptr);

	if (formatCount != 0) {
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, _surface, &formatCount, swapChainDetails.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		swapChainDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, _surface, &presentModeCount, swapChainDetails.presentModes.data());
	}

	return swapChainDetails;
}
