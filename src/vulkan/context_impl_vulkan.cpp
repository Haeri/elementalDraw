#include "context_impl_vulkan.hpp"

#include <algorithm>
#include <cstring>
#include <GLFW/glfw3.h>

#include "vulkan_shared_info.hpp"
#include "../window_impl.hpp"
#include "font_impl_vulkan.hpp"

namespace elemd
{

    /* ------------------------ DOWNCAST ------------------------ */

    inline ContextImplVulkan* getImpl(Context* ptr)
    {
        return (ContextImplVulkan*)ptr;
    }
    inline const ContextImplVulkan* getImpl(const Context* ptr)
    {
        return (const ContextImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Context* Context::create(Window* window)
    {
        return new ContextImplVulkan(window);
    }

    void Context::stroke_rect(float x, float y, float width, float height)
    {
        ContextImplVulkan* impl = getImpl(this);

        float lw = _line_width * impl->_window->_x_scale;
        float lwnorm = lw / (width * impl->_window->_x_scale + lw);
        x = x * impl->_window->_x_scale - lw/2.0f;
        y = y * impl->_window->_y_scale - lw / 2.0f;
        width = width * impl->_window->_x_scale + lw;
        height = height * impl->_window->_y_scale + lw;

        float xf = (x / impl->width);
        float yf = (y / impl->height);
        float widhtf = (width / impl->width);
        float heightf = (height / impl->height);

        impl->uniforms.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(-1, 0), vec2(0)},
             (lwnorm),
             {_stroke_color.rf(), _stroke_color.gf(), _stroke_color.bf()}});
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height,
                                      float border_radius)
    {
        stroke_rounded_rect(x, y, width, height, border_radius, border_radius, border_radius,
                            border_radius);
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height, float tl,
                                      float tr, float br, float bl)
    {
    }

    void Context::draw_point(float x, float y)
    {
    }

    void Context::stroke_line(float x, float y)
    {
    }

    void Context::stroke_circle(float x, float y, float radius)
    {
    }

    void Context::stroke_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::stroke_polygon(float x, float y)
    {
    }

    void Context::fill_rect(float x, float y, float width, float height)
    {
        ContextImplVulkan* impl = getImpl(this);
        
        x *= impl->_window->_x_scale;
        y *= impl->_window->_y_scale;
        width *= impl->_window->_x_scale;
        height *= impl->_window->_y_scale;

        float xf = (x / impl->width);
        float yf = (y / impl->height);
        float widhtf = (width / impl->width);
        float heightf = (height / impl->height);
        
        impl->uniforms.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0,0,0}});
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height,
                                    float border_radius)
    {
        fill_rounded_rect(x, y, width, height, border_radius, border_radius, border_radius,
                          border_radius);
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height, float radius_nw,
                                    float radius_ne, float radius_se, float radius_sw)
    {
        ContextImplVulkan* impl = getImpl(this);

        x *= impl->_window->_x_scale;
        y *= impl->_window->_y_scale;
        width *= impl->_window->_x_scale;
        height *= impl->_window->_y_scale;

        float xf = (x / impl->width);
        float yf = (y / impl->height);
        float widthf = (width / impl->width);
        float heightf = (height / impl->height);
        float nwxf = (radius_nw / width) * (impl->_window->_x_scale);
        float nexf = (radius_ne / width) * (impl->_window->_x_scale);
        float sexf = (radius_se / width) * (impl->_window->_x_scale);
        float swxf = (radius_sw / width) * (impl->_window->_x_scale);
        float nwyf = (radius_nw / height) * (impl->_window->_y_scale);
        float neyf = (radius_ne / height) * (impl->_window->_y_scale);
        float seyf = (radius_se / height) * (impl->_window->_y_scale);
        float swyf = (radius_sw / height) * (impl->_window->_y_scale);

        impl->uniforms.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(nwxf, nwyf), vec2(nexf, neyf), vec2(sexf, seyf), vec2(swxf, swyf)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0, 0, 0}});
    }

    void Context::fill_circle(float x, float y, float radius)
    {
        ContextImplVulkan* impl = getImpl(this);

        x *= impl->_window->_x_scale;
        y *= impl->_window->_y_scale;

        float xf = ((x - radius * impl->_window->_x_scale) / impl->width);
        float yf = ((y - radius * impl->_window->_y_scale) / impl->height);
        float widhtf = ((radius * 2 * impl->_window->_x_scale) / impl->width);
        float heightf = ((radius * 2 * impl->_window->_y_scale) / impl->height);
        float radf = 0.5f;

        impl->uniforms.push_back(
            {_fill_color,
            {vec2(xf, yf) * 2.0f - vec2(1), 
              vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(radf), vec2(radf), vec2(radf), vec2(radf)},
             {vec2(-1, 0), vec2(0)},
             0,
             {0, 0, 0}});
   
    }

    void Context::fill_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::fill_polygon(float x, float y)
    {
    }

    void Context::draw_text(float x, float y, std::string text)
    {
        ContextImplVulkan* impl = getImpl(this);

        float initialX = x;
        float scale = 0.5f;
        std::map<char, character> characters = _font->get_characters();

        for (auto& token : text)
        {
            character ch = characters[token];

            if (token == '\n')
            {
                y += _font->get_size() * scale;
                x = initialX;
                continue;
            }

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y + (64 - ch.bearing.y()) * scale;

            float width = ch.size.x() * scale;
            float height = ch.size.y() * scale;

            imageImplVulkan* img = (imageImplVulkan*)ch.texture;

            float xf = (xpos * impl->_window->_x_scale / impl->width);
            float yf = (ypos * impl->_window->_y_scale / impl->height);
            float widhtf = (width * impl->_window->_x_scale / impl->width);
            float heightf = (height * impl->_window->_y_scale / impl->height);

            impl->uniforms.push_back(
                {_fill_color,
                 {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
                  vec2(xf, yf + heightf) * 2.0f - vec2(1),
                  vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
                 {vec2(0), vec2(0), vec2(0), vec2(0)},
                 {vec2(img->_sampler_index, 1), vec2(0)},
                 0,
                 {0, 0, 0}});

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
            // pixels by 64 to get amount of pixels))
            x += (ch.advance >> 6) * scale;
        }
    }


    void Context::draw_image(float x, float y, float width, float height, image* image)
    {
        ContextImplVulkan* impl = getImpl(this);
        imageImplVulkan* img = (imageImplVulkan*)image;
        
        x *= impl->_window->_x_scale;
        y *= impl->_window->_y_scale;
        width *= impl->_window->_x_scale;
        height *= impl->_window->_y_scale;

        float xf = (x / impl->width);
        float yf = (y / impl->height);
        float widhtf = (width / impl->width);
        float heightf = (height / impl->height);

        impl->uniforms.push_back(
            {_fill_color,
             {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widhtf, yf) * 2.0f - vec2(1),
              vec2(xf, yf + heightf) * 2.0f - vec2(1),
              vec2(xf + widhtf, yf + heightf) * 2.0f - vec2(1)},
             {vec2(0), vec2(0), vec2(0), vec2(0)},
             {vec2(img->_sampler_index, 0), vec2(0)},
             0,
             {0, 0, 0}});
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, image* image,
                                     float border_radius)
    {
        draw_rounded_image(x, y, width, height, image, border_radius, border_radius, border_radius,
                           border_radius);
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, image* image, float radius_nw,
                                    float radius_ne, float radius_se, float radius_sw)
    {
        ContextImplVulkan* impl = getImpl(this);
        imageImplVulkan* img = (imageImplVulkan*)image;

        x *= impl->_window->_x_scale;
        y *= impl->_window->_y_scale;
        width *= impl->_window->_x_scale;
        height *= impl->_window->_y_scale;

        float xf = (x / impl->width);
        float yf = (y / impl->height);
        float widthf = (width / impl->width);
        float heightf = (height / impl->height);
        float nwxf = (radius_nw / width) * (impl->_window->_x_scale);
        float nexf = (radius_ne / width) * (impl->_window->_x_scale);
        float sexf = (radius_se / width) * (impl->_window->_x_scale);
        float swxf = (radius_sw / width) * (impl->_window->_x_scale);
        float nwyf = (radius_nw / height) * (impl->_window->_y_scale);
        float neyf = (radius_ne / height) * (impl->_window->_y_scale);
        float seyf = (radius_se / height) * (impl->_window->_y_scale);
        float swyf = (radius_sw / height) * (impl->_window->_y_scale);

        impl->uniforms.push_back(
            {_fill_color,
            {vec2(xf, yf) * 2.0f - vec2(1), vec2(xf + widthf, yf) * 2.0f - vec2(1),
            vec2(xf, yf + heightf) * 2.0f - vec2(1),
            vec2(xf + widthf, yf + heightf) * 2.0f - vec2(1)},
                {vec2(nwxf, nwyf), vec2(nexf, neyf), vec2(sexf, seyf), vec2(swxf, swyf)}, 
            {vec2(img->_sampler_index, 0), vec2(0)},
             0,
             {0, 0, 0}});
    }

    void Context::set_clear_color(color color)
    {
        ContextImplVulkan* impl = getImpl(this);
        _clear_color = color;
        impl->clearValue.color = {color.rf(), color.gf(), color.bf(), color.af()};
    }

    void Context::clear()
    {
    }

    void Context::clear_rect(float x, float y, float width, float height)
    {
    }

    void Context::draw_frame()
    {
        ContextImplVulkan* impl = getImpl(this);
        if (impl->rendering || impl->headless) return;
        impl->rendering = true;

        bool rerecord = false;
        if (impl->last_uniform_cnt != impl->uniforms.size())
        {
            impl->last_uniform_cnt = impl->uniforms.size();
            rerecord = true;
        }
        
        impl->wait_for_render_fence();
        impl->update_uniforms();
        if (rerecord)
        {
            impl->record_command_buffers();
        }
        impl->uniforms.clear();
       

        uint32_t imageIndex;
        vku::err_check(vkAcquireNextImageKHR(VulkanSharedInfo::getInstance()->device,
                                             impl->swapchain, std::numeric_limits<uint64_t>::max(),
                              impl->semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex));

        VkPipelineStageFlags waitStageMask[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &(impl->semaphoreImageAvailable);
        submitInfo.pWaitDstStageMask = waitStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(impl->commandBuffers[imageIndex]);
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &(impl->semaphoreRenderingComplete);

        vku::err_check(vkQueueSubmit(impl->queue, 1, &submitInfo, impl->renderFence));

        VkPresentInfoKHR presentInfoKHR;
        presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKHR.pNext = nullptr;
        presentInfoKHR.waitSemaphoreCount = 1;
        presentInfoKHR.pWaitSemaphores = &(impl->semaphoreRenderingComplete);
        presentInfoKHR.swapchainCount = 1;
        presentInfoKHR.pSwapchains = &(impl->swapchain);
        presentInfoKHR.pImageIndices = &imageIndex;
        presentInfoKHR.pResults = nullptr;

        vku::err_check(vkQueuePresentKHR(impl->queue, &presentInfoKHR));
        
        impl->rendering = false;
    }

    void Context::resize_context(int width, int height)
    {
        ContextImplVulkan* impl = getImpl(this);
        if (width == 0 || height == 0)
        {
            impl->headless = true;
        }
        else
        {
            impl->headless = false;
            impl->update_swapchain((uint32_t)width, (uint32_t)height);
        }
    }

    void Context::_tmp_register_image(image* image)
    {
        ContextImplVulkan* impl = getImpl(this);
        imageImplVulkan* img = (imageImplVulkan*)image;
        if (!img->_uploaded)
        {
            img->upload(impl->commandPool, impl->queue);
            if (impl->images.size() > TEXTURE_ARRAY_SIZE)
            {
                img->_sampler_index = 0;
                std::cerr << "Too many textures glyps loaded!" << std::endl;
            }
            else
            {
                img->_sampler_index = impl->images.size();
                impl->images.push_back(img);
            }
        }
    }

    void Context::_tmp_register_font(font* font)
    {
        ContextImplVulkan* impl = getImpl(this);
        fontImplVulkan* fiv = (fontImplVulkan*)font;
        if (!fiv->_uploaded)
        {
            fiv->upload(impl->commandPool, impl->queue);
            for (auto& c : fiv->get_characters())
            {
                imageImplVulkan* img = (imageImplVulkan*)c.second.texture;
                if (impl->images.size() > TEXTURE_ARRAY_SIZE)
                {
                    img->_sampler_index = 0;
                    std::cerr << "Too many font glyps loaded!" << std::endl;
                }
                else
                {
                    img->_sampler_index = impl->images.size();
                    impl->images.push_back(img);
                }

            }
        }
    }

    void Context::_tmp_prepare()
    {
        _default_font = font::create("./elemd_res/font/OpenSans.ttf");
        _tmp_register_font(_default_font);
        set_font(_default_font);

        ContextImplVulkan* impl = getImpl(this);
        
        impl->create_vertex_buffers();
        impl->create_index_buffers();
        impl->create_uniform_buffer();
        impl->create_descriptor_pool();
        impl->create_descriptor_set();
        impl->record_command_buffers();
    }

    void ContextImplVulkan::destroy()
    {
        _default_font->destroy();

        delete this;
    }

    /* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */

    void ContextImplVulkan::create_surface()
    {
        // --------------- Create WIndow Surface ---------------

        vku::err_check(
            glfwCreateWindowSurface(VulkanSharedInfo::getInstance()->instance, _window->getGLFWWindow(), nullptr, &surface));
    }

    void ContextImplVulkan::create_queue()
    {
        // --------------- Get Device Queue ---------------

        vkGetDeviceQueue(VulkanSharedInfo::getInstance()->device,
                         VulkanSharedInfo::getInstance()->queueFamilyIndex, 0,
                         &queue);
    }
    
    void ContextImplVulkan::check_surface_support()
    {
        // --------------- Check Surface Support ---------------

        VkBool32 surfaceSupport = false;
        vku::err_check(vkGetPhysicalDeviceSurfaceSupportKHR(
            VulkanSharedInfo::getInstance()->bestPhysicalDevice,
            VulkanSharedInfo::getInstance()->queueFamilyIndex, surface,
                                                            &surfaceSupport));
        if (!surfaceSupport)
        {
            std::cerr << "Error: Swapchain not supported on your Device!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void ContextImplVulkan::create_swapchain()
    {
        // --------------- Get Surface Capabilities ---------------

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vku::err_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            VulkanSharedInfo::getInstance()->bestPhysicalDevice, surface, &surfaceCapabilities));


        // --------------- Get Surface Formats ---------------

        uint32_t surfaceFormatCount = 0;
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanSharedInfo::getInstance()->bestPhysicalDevice,
                                                            surface, &surfaceFormatCount, nullptr));
        VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
            VulkanSharedInfo::getInstance()->bestPhysicalDevice, surface, &surfaceFormatCount, surfaceFormats));


        // --------------- Select Image Count ---------------

        uint32_t selectedImageCount = surfaceCapabilities.minImageCount + 1;
        if (selectedImageCount > surfaceCapabilities.maxImageCount)
            selectedImageCount = surfaceCapabilities.maxImageCount;


        // --------------- Select Image Format and Colorspace ---------------

        selectedImageFormat = surfaceFormats[0].format;
        VkColorSpaceKHR selectedImageColorSpace = surfaceFormats[0].colorSpace;
        for (uint32_t i = 0; i < surfaceFormatCount; ++i)
        {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
                surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                selectedImageFormat = surfaceFormats[i].format;
                selectedImageColorSpace = surfaceFormats[i].colorSpace;
            }
        }


        // --------------- Select Extents ---------------    

        VkExtent2D selectedImageExtent = {width, height};
        selectedImageExtent.width =
            std::max(surfaceCapabilities.minImageExtent.width,
                     std::min(surfaceCapabilities.maxImageExtent.width, selectedImageExtent.width));
        selectedImageExtent.height =
            std::max(surfaceCapabilities.minImageExtent.height,
                     std::min(surfaceCapabilities.maxImageExtent.height, selectedImageExtent.height));


        // --------------- Get Present Modes ---------------

        uint32_t presentModeCount = 0;
        vku::err_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
            VulkanSharedInfo::getInstance()->bestPhysicalDevice, surface, &presentModeCount, nullptr));
        VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
        vku::err_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
            VulkanSharedInfo::getInstance()->bestPhysicalDevice, surface, &presentModeCount, presentModes));


        // --------------- Select Present Mode ---------------
            
        VkPresentModeKHR selectedPresentMode =
            vku::select_present_mode(presentModes, presentModeCount, _window->get_vsync());


        // --------------- Create Swapchain Create Info ---------------

        VkSwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext = nullptr;
        swapchainCreateInfo.flags = 0;
        swapchainCreateInfo.surface = surface;
        swapchainCreateInfo.minImageCount = selectedImageCount;
        swapchainCreateInfo.imageFormat = selectedImageFormat;
        swapchainCreateInfo.imageColorSpace = selectedImageColorSpace;
        swapchainCreateInfo.imageExtent = selectedImageExtent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = selectedPresentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = swapchain;


        // --------------- Create Swapchain ---------------

        vku::err_check(vkCreateSwapchainKHR(VulkanSharedInfo::getInstance()->device,
                                            &swapchainCreateInfo, nullptr, &swapchain));


        // --------------- Cleanup ---------------

        delete[] presentModes;
        delete[] surfaceFormats;
    }

    void ContextImplVulkan::create_image_views()
    {
        // --------------- Get Swapchain Image ---------------

        vku::err_check(vkGetSwapchainImagesKHR(VulkanSharedInfo::getInstance()->device, swapchain,
                                               &VulkanSharedInfo::getInstance()->actualSwapchainImageCount, nullptr));
        VkImage* swapchainImages = new VkImage[VulkanSharedInfo::getInstance()->actualSwapchainImageCount];
        vku::err_check(vkGetSwapchainImagesKHR(VulkanSharedInfo::getInstance()->device, swapchain, &VulkanSharedInfo::getInstance()->actualSwapchainImageCount,
                                               swapchainImages));

        
        // --------------- Create Image Views ---------------

        imageViews = new VkImageView[VulkanSharedInfo::getInstance()->actualSwapchainImageCount];
        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            VkImageViewCreateInfo imageViewCreateInfo;
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = swapchainImages[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = selectedImageFormat;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            vku::err_check(
                vkCreateImageView(VulkanSharedInfo::getInstance()->device, &imageViewCreateInfo, nullptr, &imageViews[i]));
        }

        
        // --------------- Cleanup ---------------

        delete[] swapchainImages;
    }

    void ContextImplVulkan::create_render_pass()
    {
        // --------------- Create Attachment Description ---------------

        VkAttachmentDescription attachmentDescription;
        attachmentDescription.flags = 0;
        attachmentDescription.format = selectedImageFormat;
        attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        // --------------- Create Attachment Reference ---------------

        VkAttachmentReference attachmentReference;
        attachmentReference.attachment = 0;
        attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        
        // --------------- Create Subpass Desciption ---------------

        VkSubpassDescription subpassDescription;
        subpassDescription.flags = 0;
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.inputAttachmentCount = 0;
        subpassDescription.pInputAttachments = nullptr;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &attachmentReference;
        subpassDescription.pResolveAttachments = nullptr;
        subpassDescription.pDepthStencilAttachment = nullptr;
        subpassDescription.preserveAttachmentCount = 0;
        subpassDescription.pPreserveAttachments = nullptr;


        // --------------- Create Subpass Dependency ---------------

        VkSubpassDependency subpassDependency;
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dependencyFlags = 0;

        
        // --------------- Create Render Pass Create Info ---------------

        VkRenderPassCreateInfo renderPassCreateInfo;
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.pNext = nullptr;
        renderPassCreateInfo.flags = 0;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &attachmentDescription;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpassDependency;


        // --------------- Create Render Pass ---------------

        vku::err_check(vkCreateRenderPass(VulkanSharedInfo::getInstance()->device, &renderPassCreateInfo, nullptr, &renderPass));
    }

    void ContextImplVulkan::create_descriptor_set_layout()
    {
        VkDescriptorSetLayoutBinding uniformDescriptorSetLayoutBinding;
        uniformDescriptorSetLayoutBinding.binding = 0;
        uniformDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniformDescriptorSetLayoutBinding.descriptorCount = 1;
        uniformDescriptorSetLayoutBinding.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        uniformDescriptorSetLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerDescriptorSetLayoutBinding;
        samplerDescriptorSetLayoutBinding.binding = 1;
        samplerDescriptorSetLayoutBinding.descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerDescriptorSetLayoutBinding.descriptorCount = TEXTURE_ARRAY_SIZE;
        samplerDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerDescriptorSetLayoutBinding.pImmutableSamplers = nullptr;

        std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings = {
            uniformDescriptorSetLayoutBinding, 
            samplerDescriptorSetLayoutBinding
        };

        /* Since we are preallocating a big array of samplers and only supplying actually less,
        * we have to tell Vulkan, that those slots are partially bound or else it will complain.
        * This only applies to the samplerDescriptor and not uniformDescriptor, hence it being 0 as in default.
        */
        std::vector<VkDescriptorBindingFlags> descriptorBindingFlags = {
            0,
            VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT
        };

        VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorSetLayoutBindingFlagsCreateInfo;
        descriptorSetLayoutBindingFlagsCreateInfo.sType =
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
        descriptorSetLayoutBindingFlagsCreateInfo.pNext = nullptr;
        descriptorSetLayoutBindingFlagsCreateInfo.bindingCount = descriptorBindingFlags.size();
        descriptorSetLayoutBindingFlagsCreateInfo.pBindingFlags = descriptorBindingFlags.data();
        

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.pNext = &descriptorSetLayoutBindingFlagsCreateInfo;
        descriptorSetLayoutCreateInfo.flags = 0;
        descriptorSetLayoutCreateInfo.bindingCount = descriptorSetLayoutBindings.size();
        descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();

        vku::err_check(vkCreateDescriptorSetLayout(VulkanSharedInfo::getInstance()->device,
                                                   &descriptorSetLayoutCreateInfo, nullptr,
                                                   &descriptorSetLayout));
    }

    void ContextImplVulkan::create_pipeline()
    {
        // --------------- Load Shader ---------------

        fragShaderModule = new VkShaderModule();
        vertShaderModule = new VkShaderModule();
        vku::create_shader_module("./elemd_res/shader/rounded_rect.frag.spv", fragShaderModule);
        vku::create_shader_module("./elemd_res/shader/rounded_rect.vert.spv", vertShaderModule);


        // --------------- Create Pipeline Shader Stage Create Info ---------------

        VkPipelineShaderStageCreateInfo shaderStageCreateInfoVert;
        shaderStageCreateInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfoVert.pNext = nullptr;
        shaderStageCreateInfoVert.flags = 0;
        shaderStageCreateInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageCreateInfoVert.module = *vertShaderModule;
        shaderStageCreateInfoVert.pName = "main";
        shaderStageCreateInfoVert.pSpecializationInfo = nullptr;

        
        // --------------- Create Pipeline Shader Stage Create Info ---------------

        VkPipelineShaderStageCreateInfo shaderStageCreateInfoFrag;
        shaderStageCreateInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfoFrag.pNext = nullptr;
        shaderStageCreateInfoFrag.flags = 0;
        shaderStageCreateInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageCreateInfoFrag.module = *fragShaderModule;
        shaderStageCreateInfoFrag.pName = "main";
        shaderStageCreateInfoFrag.pSpecializationInfo = nullptr;

        VkPipelineShaderStageCreateInfo shaderStages[] = {shaderStageCreateInfoVert,
                                                          shaderStageCreateInfoFrag};


        /*
        VkVertexInputBindingDescription vertexInputBindingDescription = vertex::getBindingDescription();
        std::array<VkVertexInputAttributeDescription, 1> vertexInputAttributeDescription =
            vertex::gerAttributeDescriptions();
            */
        
        // --------------- Create Pipeline Vertex Input State Create Info ---------------

        VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
        pipelineVertexInputStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pipelineVertexInputStateCreateInfo.pNext = nullptr;
        pipelineVertexInputStateCreateInfo.flags = 0;
        /*
        pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
        pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions =
            &vertexInputBindingDescription;
        pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount =
            (uint32_t)vertexInputAttributeDescription.size();
        pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions =
            vertexInputAttributeDescription.data();
        */
        pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
        pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr; 
        pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
        pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;


        // --------------- Create Pipeline Input Assembly State Create Info ---------------

        VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;
        pipelineInputAssemblyStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
        pipelineInputAssemblyStateCreateInfo.flags = 0;
        pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


        // --------------- Create Pipeline Viewport State Create Info ---------------

        VkViewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)width;
        viewport.height = (float)height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = {width, height};

        VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
        pipelineViewportStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipelineViewportStateCreateInfo.pNext = nullptr;
        pipelineViewportStateCreateInfo.flags = 0;
        pipelineViewportStateCreateInfo.viewportCount = 1;
        pipelineViewportStateCreateInfo.pViewports = &viewport;
        pipelineViewportStateCreateInfo.scissorCount = 1;
        pipelineViewportStateCreateInfo.pScissors = &scissor;


        // --------------- Create Pipeline Rasterization State Create Info ---------------

        VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
        pipelineRasterizationStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipelineRasterizationStateCreateInfo.pNext = nullptr;
        pipelineRasterizationStateCreateInfo.flags = 0;
        pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
        pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
        pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
        pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;


        // --------------- Create Pipeline Multisample State Create Info ---------------

        VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
        pipelineMultisampleStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipelineMultisampleStateCreateInfo.pNext = nullptr;
        pipelineMultisampleStateCreateInfo.flags = 0;
        pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        pipelineMultisampleStateCreateInfo.minSampleShading = 1.0;
        pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
        pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_TRUE;
        pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

        
        // --------------- Create Pipeline Color Blend Attachment State ---------------

        VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
        pipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
        pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
        pipelineColorBlendAttachmentState.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;


        // --------------- Create Pipeline Color Blend State Create Info ---------------

        VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
        pipelineColorBlendStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipelineColorBlendStateCreateInfo.pNext = nullptr;
        pipelineColorBlendStateCreateInfo.flags = 0;
        pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
        pipelineColorBlendStateCreateInfo.attachmentCount = 1;
        pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
        pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
        pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;


        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;
        pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipelineDynamicStateCreateInfo.pNext = nullptr;
        pipelineDynamicStateCreateInfo.flags = 0;
        pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
        pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates;

        // --------------- Create Pipeline Layout Create Info ---------------

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pNext = nullptr;
        pipelineLayoutCreateInfo.flags = 0;
        pipelineLayoutCreateInfo.setLayoutCount = 1;
        pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;


        // --------------- Create Pipeline Layout ---------------

        vku::err_check(
            vkCreatePipelineLayout(VulkanSharedInfo::getInstance()->device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));


        // --------------- Create Graphics Pipeline Create Info ---------------

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.pNext = nullptr;
        graphicsPipelineCreateInfo.flags = 0;
        graphicsPipelineCreateInfo.stageCount = 2;
        graphicsPipelineCreateInfo.pStages = shaderStages;
        graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
        graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
        graphicsPipelineCreateInfo.pTessellationState = nullptr;
        graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
        graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
        graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
        graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
        graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
        graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
        graphicsPipelineCreateInfo.layout = pipelineLayout;
        graphicsPipelineCreateInfo.renderPass = renderPass;
        graphicsPipelineCreateInfo.subpass = 0;
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        graphicsPipelineCreateInfo.basePipelineIndex = -1;


        // --------------- Create Graphics Pipeline ---------------

        vku::err_check(vkCreateGraphicsPipelines(VulkanSharedInfo::getInstance()->device, VK_NULL_HANDLE, 1,
                                                 &graphicsPipelineCreateInfo, nullptr, &pipeline));
    }

    void ContextImplVulkan::create_framebuffer()
    {
        // --------------- Create Framebuffers ---------------

        frameBuffers = new VkFramebuffer[VulkanSharedInfo::getInstance()->actualSwapchainImageCount];

        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            VkFramebufferCreateInfo frameBufferCreateInfo;
            frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frameBufferCreateInfo.pNext = nullptr;
            frameBufferCreateInfo.flags = 0;
            frameBufferCreateInfo.renderPass = renderPass;
            frameBufferCreateInfo.attachmentCount = 1;
            frameBufferCreateInfo.pAttachments = &(imageViews[i]);
            frameBufferCreateInfo.width = width;
            frameBufferCreateInfo.height = height;
            frameBufferCreateInfo.layers = 1;

            vku::err_check(
                vkCreateFramebuffer(VulkanSharedInfo::getInstance()->device, &frameBufferCreateInfo, nullptr, &(frameBuffers[i])));
        }
    }

    void ContextImplVulkan::create_command_pool()
    {
        // --------------- Create Command Pool Create Info ---------------

        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = VulkanSharedInfo::getInstance()->queueFamilyIndex;


        // --------------- Create Command Pool ---------------

        vku::err_check(vkCreateCommandPool(VulkanSharedInfo::getInstance()->device, &commandPoolCreateInfo, nullptr, &commandPool));
    }

    void ContextImplVulkan::create_command_buffers()
    {
        // --------------- Create Command Buffer Allocate Info ---------------

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = VulkanSharedInfo::getInstance()->actualSwapchainImageCount;


        // --------------- Allocate Command Buffers ---------------

        commandBuffers = new VkCommandBuffer[VulkanSharedInfo::getInstance()->actualSwapchainImageCount];
        vku::err_check(
            vkAllocateCommandBuffers(VulkanSharedInfo::getInstance()->device, &commandBufferAllocateInfo, commandBuffers));
    }

    void ContextImplVulkan::record_command_buffers()
    {
        // --------------- Create Command Buffer Begin Info ---------------

        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            vku::err_check(vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo));

            VkRenderPassBeginInfo renderPassBeginInfo;
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.pNext = nullptr;
            renderPassBeginInfo.renderPass = renderPass;
            renderPassBeginInfo.framebuffer = frameBuffers[i];
            renderPassBeginInfo.renderArea.offset = {0, 0};
            renderPassBeginInfo.renderArea.extent = {width, height};
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = &clearValue;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo,
                                 VK_SUBPASS_CONTENTS_INLINE);

                vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

                VkViewport viewport;
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float)width;
                viewport.height = (float)height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                VkRect2D scissor;
                scissor.offset = { 0, 0 };
                scissor.extent = { width, height };

                vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);
                vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);

                VkDeviceSize offsets[] = { 0 };
                //vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &vertexBuffer, offsets);
                vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
            
                vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

                vkCmdDrawIndexed(commandBuffers[i], (uint32_t)rect_indices.size(),
                                 uniforms.size(), 0, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            vku::err_check(vkEndCommandBuffer(commandBuffers[i]));
        }
    }

    void ContextImplVulkan::wait_for_render_fence()
    {
        vku::err_check(vkWaitForFences(VulkanSharedInfo::getInstance()->device, 1, &renderFence,
                                       true, 1000000000));
        vku::err_check(vkResetFences(VulkanSharedInfo::getInstance()->device, 1, &renderFence));
    }

    void ContextImplVulkan::create_semaphores()
    {

        VkFenceCreateInfo fenceCreateInfo;
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        vku::err_check(vkCreateFence(VulkanSharedInfo::getInstance()->device, &fenceCreateInfo,
                                     nullptr, &renderFence));


        // --------------- Create Semaphore Create Info ---------------

        VkSemaphoreCreateInfo semaphoreCreateInfo;
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;


        // --------------- Create Semaphores ---------------

        vku::err_check(
            vkCreateSemaphore(VulkanSharedInfo::getInstance()->device, &semaphoreCreateInfo, nullptr, &semaphoreImageAvailable));
        vku::err_check(
            vkCreateSemaphore(VulkanSharedInfo::getInstance()->device, &semaphoreCreateInfo, nullptr, &semaphoreRenderingComplete));
    }

    void ContextImplVulkan::initialize_resources()
    {
        uint8_t* buffer = new uint8_t[4];
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        dummy = new imageImplVulkan(1, 1, 4, buffer);
        dummy->upload(commandPool, queue);
    }

    void ContextImplVulkan::update_swapchain(uint32_t width, uint32_t height)
    {
        if (resizing) return;
        resizing = true;

        this->width = width;
        //*_window->_x_scale;
        this->height = height;
        //*_window->_y_scale;        

        vkDeviceWaitIdle(VulkanSharedInfo::getInstance()->device);

        vkFreeCommandBuffers(VulkanSharedInfo::getInstance()->device, commandPool, VulkanSharedInfo::getInstance()->actualSwapchainImageCount, commandBuffers);
        vkDestroyCommandPool(VulkanSharedInfo::getInstance()->device, commandPool, nullptr);
        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            vkDestroyFramebuffer(VulkanSharedInfo::getInstance()->device, frameBuffers[i], nullptr);
        }
        vkDestroyRenderPass(VulkanSharedInfo::getInstance()->device, renderPass, nullptr);
        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            vkDestroyImageView(VulkanSharedInfo::getInstance()->device, imageViews[i], nullptr);
        }
        
        delete[] commandBuffers;
        delete[] frameBuffers;
        delete[] imageViews;

        VkSwapchainKHR oldSwapchain = swapchain;

        create_swapchain();
        create_image_views();
        create_render_pass();
        create_framebuffer();
        create_command_pool();
        create_command_buffers();
        record_command_buffers();

        vkDestroySwapchainKHR(VulkanSharedInfo::getInstance()->device, oldSwapchain, nullptr);

        resizing = false;
    }

    ContextImplVulkan::ContextImplVulkan(Window* window)
    {
        _window = (WindowImpl*)window;

        width = (uint32_t)window->get_width();
        height = (uint32_t)window->get_height();        


        VulkanSharedInfo::getInstance();
        create_surface();
        check_surface_support();
        create_queue();

        create_swapchain();
        create_image_views();
        create_render_pass();

        create_descriptor_set_layout();

        create_pipeline();
        
        create_framebuffer();
        create_command_pool();
        create_command_buffers();
        initialize_resources();

        create_semaphores();

#ifndef NDEBUG
        std::cout << "{\n";
        vku::print_layers();
        //vku::print_extensions();
        vku::print_physical_devices();
        vku::print_selected_device();
        std::cout << "}\n";      
#endif
    }

    ContextImplVulkan::~ContextImplVulkan()
    {
        vkDeviceWaitIdle(VulkanSharedInfo::getInstance()->device);

        vkDestroyDescriptorSetLayout(VulkanSharedInfo::getInstance()->device, descriptorSetLayout,
                                     nullptr);
        vkDestroyDescriptorPool(VulkanSharedInfo::getInstance()->device, descriptorPool, nullptr);
        vkFreeMemory(VulkanSharedInfo::getInstance()->device, uniformBufferDeviceMemory, nullptr);
        vkDestroyBuffer(VulkanSharedInfo::getInstance()->device, uniformBuffer, nullptr);

        vkDestroyFence(VulkanSharedInfo::getInstance()->device, renderFence, nullptr);
        vkDestroySemaphore(VulkanSharedInfo::getInstance()->device, semaphoreImageAvailable, nullptr);
        vkDestroySemaphore(VulkanSharedInfo::getInstance()->device, semaphoreRenderingComplete, nullptr);

        vkFreeMemory(VulkanSharedInfo::getInstance()->device, indexBufferDeviceMemory, nullptr);
        vkDestroyBuffer(VulkanSharedInfo::getInstance()->device, indexBuffer, nullptr);

//        vkFreeMemory(VulkanSharedInfo::getInstance()->device, vertexBufferDeviceMemory, nullptr);
//        vkDestroyBuffer(VulkanSharedInfo::getInstance()->device, vertexBuffer, nullptr);  

        vkFreeCommandBuffers(VulkanSharedInfo::getInstance()->device, commandPool,
                             VulkanSharedInfo::getInstance()->actualSwapchainImageCount,
                             commandBuffers);
        vkDestroyCommandPool(VulkanSharedInfo::getInstance()->device, commandPool, nullptr);
        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
          vkDestroyFramebuffer(VulkanSharedInfo::getInstance()->device, frameBuffers[i], nullptr);
        }

        vkDestroyPipeline(VulkanSharedInfo::getInstance()->device, pipeline, nullptr);
        
        vkDestroyRenderPass(VulkanSharedInfo::getInstance()->device, renderPass, nullptr);
        
        vkDestroyPipelineLayout(VulkanSharedInfo::getInstance()->device, pipelineLayout, nullptr);

        vkDestroyShaderModule(VulkanSharedInfo::getInstance()->device, *vertShaderModule, nullptr);
        vkDestroyShaderModule(VulkanSharedInfo::getInstance()->device, *fragShaderModule, nullptr);
        
        for (uint32_t i = 0; i < VulkanSharedInfo::getInstance()->actualSwapchainImageCount; ++i)
        {
            vkDestroyImageView(VulkanSharedInfo::getInstance()->device, imageViews[i], nullptr);
        }
        
        vkDestroySwapchainKHR(VulkanSharedInfo::getInstance()->device, swapchain, nullptr);
        vkDestroySurfaceKHR(VulkanSharedInfo::getInstance()->instance, surface, nullptr);

        
        delete[] commandBuffers;
        delete[] frameBuffers;
        delete vertShaderModule;
        delete fragShaderModule;
        delete[] imageViews;

        delete dummy;
    }


    void ContextImplVulkan::create_vertex_buffers()
    {
        /*
        vku::create_and_upload_buffer(rect_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                      vertexBuffer, vertexBufferDeviceMemory, commandPool, queue);
        */
    }

     void ContextImplVulkan::create_index_buffers()
    {
        vku::create_and_upload_buffer(rect_indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBuffer,
                                      indexBufferDeviceMemory, commandPool, queue);
    }


    void ContextImplVulkan::create_uniform_buffer()
    {
        VkDeviceSize bufferSize = UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE;
        vku::create_buffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, uniformBuffer,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           uniformBufferDeviceMemory);
    }

    void ContextImplVulkan::create_descriptor_pool()
    {
        VkDescriptorPoolSize uniformDescriptorPoolSize;
        uniformDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniformDescriptorPoolSize.descriptorCount = 1;

        VkDescriptorPoolSize samplerDescriptorPoolSize;
        samplerDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerDescriptorPoolSize.descriptorCount = 1;

        std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {
            uniformDescriptorPoolSize,
            samplerDescriptorPoolSize
        };

        VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolCreateInfo.pNext = nullptr;
        descriptorPoolCreateInfo.flags = 0;
        descriptorPoolCreateInfo.maxSets = 1;
        descriptorPoolCreateInfo.poolSizeCount = descriptorPoolSizes.size();
        descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

        vku::err_check(vkCreateDescriptorPool(VulkanSharedInfo::getInstance()->device,
                                              &descriptorPoolCreateInfo, nullptr, &descriptorPool));
    }

    void ContextImplVulkan::create_descriptor_set()
    {
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
        descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        descriptorSetAllocateInfo.pNext = nullptr;
        descriptorSetAllocateInfo.descriptorPool = descriptorPool;
        descriptorSetAllocateInfo.descriptorSetCount = 1;
        descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

        vku::err_check(vkAllocateDescriptorSets(VulkanSharedInfo::getInstance()->device,
                                                &descriptorSetAllocateInfo, &descriptorSet));

        VkDescriptorBufferInfo descriptorBufferInfo;
        descriptorBufferInfo.buffer = uniformBuffer;
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = UNIFORM_RECT_BUFFER_ARRAY_MAX_SIZE;
        
        VkWriteDescriptorSet uniformWriteDescriptorSet;
        uniformWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        uniformWriteDescriptorSet.pNext = nullptr;
        uniformWriteDescriptorSet.dstSet = descriptorSet;
        uniformWriteDescriptorSet.dstBinding = 0;
        uniformWriteDescriptorSet.dstArrayElement = 0;
        uniformWriteDescriptorSet.descriptorCount = 1;
        uniformWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniformWriteDescriptorSet.pImageInfo = nullptr;
        uniformWriteDescriptorSet.pBufferInfo = &descriptorBufferInfo;
        uniformWriteDescriptorSet.pTexelBufferView = nullptr;


        
        std::vector<VkDescriptorImageInfo> descriptorImageInfos;
        //[images.size()];

        if (images.size() <= 0)
        {
            VkDescriptorImageInfo descriptorImageInfo;
            descriptorImageInfo.sampler = dummy->_sampler;
            descriptorImageInfo.imageView = dummy->_imageView;
            descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            descriptorImageInfos.push_back(descriptorImageInfo);
        }
        else
        {        
            for (uint32_t i = 0; i < images.size(); ++i)
            {
                VkDescriptorImageInfo descriptorImageInfo;
                descriptorImageInfo.sampler = images[i]->_sampler;
                descriptorImageInfo.imageView = images[i]->_imageView;
                descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                descriptorImageInfos.push_back(descriptorImageInfo);
            }
        }

        /*
        for (uint32_t i = 0; i < images.size(); ++i)
        {
            if (i < images.size())
            {
                descriptorImageInfos[i].sampler = images[i]->_sampler;
                descriptorImageInfos[i].imageView = images[i]->_imageView;
            }
            else
            {
                descriptorImageInfos[i].sampler = dummy->_sampler;
                descriptorImageInfos[i].imageView = dummy->_imageView;
            }
            descriptorImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }*/

        VkWriteDescriptorSet samplerWriteDescriptorSet;
        samplerWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        samplerWriteDescriptorSet.pNext = nullptr;
        samplerWriteDescriptorSet.dstSet = descriptorSet;
        samplerWriteDescriptorSet.dstBinding = 1;
        samplerWriteDescriptorSet.dstArrayElement = 0;
        samplerWriteDescriptorSet.descriptorCount = descriptorImageInfos.size();
        samplerWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerWriteDescriptorSet.pImageInfo = descriptorImageInfos.data();
        samplerWriteDescriptorSet.pBufferInfo = nullptr;
        samplerWriteDescriptorSet.pTexelBufferView = nullptr;


        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            uniformWriteDescriptorSet, 
            samplerWriteDescriptorSet
        };

        vkUpdateDescriptorSets(VulkanSharedInfo::getInstance()->device, writeDescriptorSets.size(),
                               writeDescriptorSets.data(), 0, nullptr);
    }

    void ContextImplVulkan::update_uniforms()
    {
        VkDeviceSize bufferSize = sizeof(uniform_rect) * uniforms.size();

        void* rawData;
        vkMapMemory(VulkanSharedInfo::getInstance()->device, uniformBufferDeviceMemory, 0,
                    bufferSize, 0, &rawData);

        std::memcpy(rawData, uniforms.data(), bufferSize);
        vkUnmapMemory(VulkanSharedInfo::getInstance()->device, uniformBufferDeviceMemory);





    }

} // namespace elemd 