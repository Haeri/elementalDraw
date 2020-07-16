#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>
#include <atomic>

#include "vulkan_utils.hpp"
#include "../window_impl.hpp"

namespace elemd
{
    class VulkanContext : public Context
    {
    public:

        VulkanContext(Window* window);
        ~VulkanContext();

        void preload_vulkan();
        void create_instance();
        void create_surface();
        void create_physical_devices();
        void create_logical_device();
        void load_vulkan();
        void create_queue();
        void create_swapchain();
        void check_surface_support();
        void create_image_views();
        void create_render_pass();
        void create_pipeline();
        void create_framebuffer();
        void create_command_pool();
        void create_command_buffers();
        void record_command_buffers();
        void create_semaphores();

        void regenerate_swapchain(uint32_t width, uint32_t height);

        void create_shader_module(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> read_shader(const std::string& filename);

        WindowImpl* _window;

        std::atomic<bool> resizing = false;
        std::atomic<bool> rendering = false;

        uint32_t width = 0;
        uint32_t height = 0;

        uint32_t actualSwapchainImageCount = 0;
        uint32_t physicalDeviceCount = 0;
        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 0.3f};

        PhysicalDeviceComposite bestDevice;
        VkFormat selectedImageFormat;

        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice* physicalDevices;
        VkDevice device;
        VkQueue queue;
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
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