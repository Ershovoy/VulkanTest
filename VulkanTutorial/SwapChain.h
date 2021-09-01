#pragma once

#include "Device.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

private:
	VkSwapchainKHR _swapchain;
	std::vector<VkImage> _images;
	std::vector<VkImageView> _imageViews;
	std::vector<VkFramebuffer> _framebuffers;

	VkSemaphore _imageAvailableSemaphore;
	VkSemaphore _renderFinishedSemaphore;

	VkFence _fence;
};

