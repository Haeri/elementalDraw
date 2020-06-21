#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemental_draw/context.hpp"

#include <string>

#include "vulkan_utils.hpp"

class VulkanContext : public Context 
{
public:
    VulkanContext(Window* window);
	~VulkanContext();

    VkSurfaceKHR* _vulkanSurface;
	VkInstance* _vulkanInstance;
	VkDevice* _vulkanDevice;
};

#endif // ELEMD_VULKAN_CONTEXT_HPP