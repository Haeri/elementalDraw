#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

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


        uint32_t actualSwapchainImageCount = 0;

        VkDevice* _vulkanDevice;
        VkInstance* _vulkanInstance;
        VkSurfaceKHR* _vulkanSurface;
        VkSwapchainKHR* _vulkanSwapchain;
        VkImageView* _vulkanImageViews;
        VkShaderModule fragShaderModule;
        VkShaderModule vertShaderModule;
        VkPipelineLayout* _vulkanPipelineLayout;
        VkRenderPass* _vulkanRenderPass;
        VkPipeline* _vulkanPipeline;
        VkFramebuffer* _vulkanFrameBuffers;
        VkCommandPool* _vulkanCommandPool;
        VkCommandBuffer* _vulkanCommandBuffers;
        VkClearValue _clearValue = {0.0f, 0.0f, 0.0f, 1.0f};
    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP