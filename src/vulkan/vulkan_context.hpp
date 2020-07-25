#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>
#include <array>
#include <atomic>
#include <cstring>

#include "elemd/vec2.hpp"
#include "elemd/color.hpp"
#include "vertex.hpp"
#include "vulkan_shared_info.hpp"
#include "vulkan_utils.hpp"
#include "../window_impl.hpp"

namespace elemd
{
    class VulkanContext : public Context
    {
    public:
        const uint32_t UNIFORM_BUFFER_ARRAY_MAX_COUNT = 20;

        struct uniform_rect
        {
            vec2 vertices[4];
            vec2 border_radius[4];
        };

        std::vector<vertex> rect_vertices = {{vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}};
        std::vector<uint32_t> rect_indices = {0, 1, 2, 1, 3, 2};

        std::vector<uniform_rect> uniforms = {};

        WindowImpl* _window;

        std::atomic<bool> resizing = false;
        std::atomic<bool> rendering = false;

        uint32_t width = 0;
        uint32_t height = 0;

        VkClearValue clearValue = {};
        VkFormat selectedImageFormat;

        VkSurfaceKHR surface;
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

        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferDeviceMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferDeviceMemory;

        VkBuffer uniformBuffer;
        VkDeviceMemory uniformBufferDeviceMemory;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        VkDescriptorSet descriptorSet;

        VkSemaphore semaphoreImageAvailable;
        VkSemaphore semaphoreRenderingComplete;

        VulkanContext(Window* window);
        ~VulkanContext();

        void create_surface();
        void create_queue();
        void create_swapchain();
        void check_surface_support();
        void create_image_views();
        void create_render_pass();
        void create_descriptor_set_layout();
        void create_pipeline();
        void create_framebuffer();
        void create_command_pool();
        void create_command_buffers();
        void create_mesh_buffers();
        void create_uniform_buffer();
        void create_descriptor_pool();
        void create_descriptor_set();
        void record_command_buffers();
        void create_semaphores();
        void destroy_mesh_buffers();

        void update_uniforms();

        void regenerate_swapchain(uint32_t width, uint32_t height);

        void create_shader_module(const std::string& filename, VkShaderModule* shaderModule);
        std::vector<char> read_shader(const std::string& filename);

    private:
        template<typename T>
        void create_and_upload_buffer(std::vector<T> data, const VkBufferUsageFlags& usageFlags, VkBuffer& buffer, VkDeviceMemory& deviceMemory) {

            // --------------- Create Staging Buffer and Memory ---------------

            VkDeviceSize bufferSize = sizeof(T) * data.size();
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferDeviceMemory;

            vku::create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, stagingBuffer,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               stagingBufferDeviceMemory);

            void* rawData;
            vkMapMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory, 0,
                        bufferSize, 0, &rawData);
            std::memcpy(rawData, data.data(), bufferSize);
            vkUnmapMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory);

            // --------------- Create Buffer and Memory ---------------

            vku::create_buffer(bufferSize, usageFlags | VK_BUFFER_USAGE_TRANSFER_DST_BIT, buffer,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, deviceMemory);

            vku::copy_buffer(stagingBuffer, buffer, bufferSize, commandPool, queue);

            // --------------- Cleanup ---------------

            vkDestroyBuffer(VulkanSharedInfo::getInstance()->device, stagingBuffer, nullptr);
            vkFreeMemory(VulkanSharedInfo::getInstance()->device, stagingBufferDeviceMemory,
                         nullptr);
        }
    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP