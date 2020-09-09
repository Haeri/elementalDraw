#ifndef ELEMD_VULKAN_CONTEXT_HPP
#define ELEMD_VULKAN_CONTEXT_HPP

#include "elemd/context.hpp"

#include <string>
#include <vector>
#include <array>
#include <atomic>

#include "elemd/vec2.hpp"
#include "elemd/color.hpp"
#include "vertex.hpp"
#include "vulkan_shared_info.hpp"
#include "vulkan_utils.hpp"
#include "../window_impl.hpp"
#include "image_impl_vulkan.hpp"

#define TEXTURE_ARRAY_SIZE 256
#define UNIFORM_BUFFER_ARRAY_MAX_COUNT 65536
#define UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE ((int)(UNIFORM_BUFFER_ARRAY_MAX_COUNT / sizeof(uniform_rect))) * sizeof(uniform_rect)


namespace elemd
{
    class ContextImplVulkan : public Context
    {
    public:            

        struct uniform_rect
        {
            color fill_color;
            vec2 vertices[4];
            vec2 border_radius[4];
            vec2 sampler_index[2];
            float line_size;
            float stroke_color[3];
        };

        imageImplVulkan* dummy;
        std::vector<imageImplVulkan*> images;

        //std::vector<vertex> rect_vertices = {{vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}};
        std::vector<uint32_t> rect_indices = {0, 1, 2, 1, 3, 2};
        
        std::vector<point_vertex> point_vertices = {
            {vec2(0)}, {vec2(1, 0)}, {vec2(0, 1)}, {vec2(1)}
        };

        std::vector<uniform_rect> uniforms = {};
        int last_uniform_cnt = 0;

        WindowImpl* _window;

        std::atomic<bool> resizing = false;
        std::atomic<bool> rendering = false;
        bool headless = false;

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

        //VkBuffer vertexBuffer = VK_NULL_HANDLE;
        //VkDeviceMemory vertexBufferDeviceMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferDeviceMemory;

        VkBuffer uniformBuffer;
        VkDeviceMemory uniformBufferDeviceMemory;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        VkDescriptorSet descriptorSet;

        VkSemaphore semaphoreImageAvailable;
        VkSemaphore semaphoreRenderingComplete;
        VkFence renderFence;

        ContextImplVulkan(Window* window);
        ~ContextImplVulkan();

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
        void create_vertex_buffers();
        void create_index_buffers();
        void create_uniform_buffer();
        void create_descriptor_pool();
        void create_descriptor_set();
        void record_command_buffers();
        void wait_for_render_fence();
        void create_semaphores();
        void initialize_resources();

        void update_uniforms();
        void update_swapchain(uint32_t width, uint32_t height);

        void destroy() override;
    };

} // namespace elemd

#endif // ELEMD_VULKAN_CONTEXT_HPP