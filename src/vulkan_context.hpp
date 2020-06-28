#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemental_draw/context.hpp"

#include <string>
#include <vector>

#include "vulkan_utils.hpp"

namespace elemd
{
    class VulkanContext : public Context
    {
    public:
        VulkanContext(Window* window);
        ~VulkanContext();

        void createShaderModule(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> readShader(const std::string& filename);

        VkShaderModule fragShaderModule;
        VkShaderModule vertShaderModule;
        uint32_t actualSwapchainImageCount = 0;
        VkImageView* _vulkanImageViews;
        VkSwapchainKHR* _vulkanSwapchain;
        VkSurfaceKHR* _vulkanSurface;
        VkInstance* _vulkanInstance;
        VkDevice* _vulkanDevice;
    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP