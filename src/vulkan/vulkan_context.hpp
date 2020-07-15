#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>

#include "vulkan_utils.hpp"
#include "../window_impl.hpp"

namespace elemd
{
    class VulkanContext : public Context
    {
    public:

        VulkanContext(Window* window);
        ~VulkanContext();

        void create_instance();
        void create_surface();
        void create_physical_devices();
        void create_logical_device();

        void create_shader_module(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> read_shader(const std::string& filename);

        WindowImpl* _window;

        uint32_t actualSwapchainImageCount = 0;
        uint32_t physicalDeviceCount = 0;
        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 0.3f};

        PhysicalDeviceComposite bestDevice;

        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice* physicalDevices;
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