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

        void createInstance();
        void createShaderModule(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> readShader(const std::string& filename);


        uint32_t actualSwapchainImageCount = 0;
        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 0.0f};

        VkInstance instance;
        VkSurfaceKHR surface;
        VkDevice device;
        VkQueue queue;
        VkSwapchainKHR swapchain;
        VkImageView* imageViews;
        VkShaderModule* fragShaderModule;
        VkShaderModule* vertShaderModule;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline pipeline;
        VkFramebuffer* frameBuffers;
        VkCommandPool commandPool;
        VkCommandBuffer* commandBuffers;

        VkSemaphore semaphoreImageAvailable;
        VkSemaphore semaphoreRenderingComplete;

    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP