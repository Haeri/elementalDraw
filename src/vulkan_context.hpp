#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemental_draw/context.hpp"

#include <string>

#include "vulkan_utils.hpp"

namespace elemd
{

    class VulkanContext : public Context
    {
    public:
        VulkanContext(Window* window);
        ~VulkanContext();

        VkSwapchainKHR* _vulkanSwapchain;
        VkSurfaceKHR* _vulkanSurface;
        VkInstance* _vulkanInstance;
        VkDevice* _vulkanDevice;
    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP