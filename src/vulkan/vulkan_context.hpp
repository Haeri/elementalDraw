#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>
#include <atomic>

#include "elemd/vec2.hpp"
#include "elemd/color.hpp"
#include "vulkan_utils.hpp"
#include "../window_impl.hpp"

namespace elemd
{
    class VulkanContext : public Context
    {
    public:

        struct vertex
        {
            vec2 pos;
            color col;

            static VkVertexInputBindingDescription getBindingDescription()
            {
                VkVertexInputBindingDescription vertexInputBindingDescription;
                vertexInputBindingDescription.binding = 0;
                vertexInputBindingDescription.stride = sizeof(vertex);
                vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return vertexInputBindingDescription;
            }

            static std::vector<VkVertexInputAttributeDescription> gerAttributeDescriptions()
            {
                std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(vertex, pos);
                
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(vertex, col);

                return attributeDescriptions;
            }
        };

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
        void create_vertex_buffer();
        void record_command_buffers();
        void create_semaphores();

        void regenerate_swapchain(uint32_t width, uint32_t height);

        void create_shader_module(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> read_shader(const std::string& filename);

        std::vector<vertex> vertices = {
            {{0.0f, -0.5f}, color(50, 130, 184, 255)},
            {{0.5f, 0.5f}, color(50, 130, 184, 255)},
            {{-0.5f, 0.5f}, color(50, 130, 184, 255)}
            /*
            {{ 0.0f, 0.0f}, color(50, 130, 184, 255)},
            {{ 0.0f,-0.5f}, color(50, 130, 184, 255)},
            {{ 0.5f, 0.0f}, color(50, 130, 184, 255)},
            {{ 0.5f,-0.5f}, color(50, 130, 184, 255)}
            */
        };

        WindowImpl* _window;

        std::atomic<bool> resizing = false;
        std::atomic<bool> rendering = false;

        uint32_t width = 0;
        uint32_t height = 0;

        uint32_t actualSwapchainImageCount = 0;
        uint32_t physicalDeviceCount = 0;
        VkClearValue clearValue = {0.0f, 1.0f, 0.0f, 1.0f};

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
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferDeviceMemory;
        VkCommandBuffer* commandBuffers;

        VkSemaphore semaphoreImageAvailable;
        VkSemaphore semaphoreRenderingComplete;

    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP