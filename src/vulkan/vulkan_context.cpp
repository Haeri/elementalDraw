#include "vulkan_context.hpp"

#include <algorithm>
#include <fstream>
#include <GLFW/glfw3.h>

#include "../window_impl.hpp"


namespace elemd
{

    /* ------------------------ DOWNCAST ------------------------ */

    inline VulkanContext* getImpl(Context* ptr)
    {
        return (VulkanContext*)ptr;
    }
    inline const VulkanContext* getImpl(const Context* ptr)
    {
        return (const VulkanContext*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Context* Context::create(Window* window)
    {
        return new VulkanContext(window);
    }

    void Context::stroke_rect(float x, float y, float width, float height)
    {
    }

    void Context::stroke_rounded_rect(float x, float y, float width, float height, float tl,
                                      float tr, float br, float bl)
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
    }

    void Context::fill_rounded_rect(float x, float y, float width, float height, float tl, float tr,
                                    float br, float bl)
    {
    }

    void Context::fill_circle(float x, float y, float radius)
    {
    }

    void Context::fill_ellipse(float x, float y, float width, float height)
    {
    }

    void Context::fill_polygon(float x, float y)
    {
    }

    void Context::draw_text(float x, float y, char* text)
    {
    }

    void Context::draw_image(float x, float y, float width, float height, uint32_t image)
    {
    }

    void Context::draw_rounded_image(float x, float y, float width, float height, uint32_t image,
                                     float tl, float tr, float br, float bl)
    {
    }

    void Context::set_clear_color(Color&& color)
    {
        VulkanContext* impl = getImpl(this);
        _clear_color = color;
        impl->clearValue = {color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f,
                            color.a() / 255.0f};
    }

    void Context::clear()
    {
    }

    void Context::clear_rect(float x, float y, float width, float height)
    {
    }

    void Context::draw_frame()
    {
        VulkanContext* impl = getImpl(this);

        uint32_t imageIndex;
        vku::err_check(vkAcquireNextImageKHR(impl->device, impl->swapchain, std::numeric_limits<uint64_t>::max(),
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

        vku::err_check(vkQueueSubmit(impl->queue, 1, &submitInfo, VK_NULL_HANDLE));

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

    }

    /* ------------------------ PRIVATE IMPLEMENTATION ------------------------ */


    void VulkanContext::preload_vulkan()
    {
        if (gladLoaderLoadVulkan(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) <= 0)
        {
            vku::err("Could not load Vulkan!");
            exit(EXIT_FAILURE);
        }
    }

    void VulkanContext::create_instance()
    {
        // --------------- Create Application Info ---------------

        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "UI Application"; // TODO: Pass down from application
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = ELEMD_LIBRARY_NAME;
        applicationInfo.engineVersion =
            VK_MAKE_VERSION(ELEMD_VERSION_MAJOR, ELEMD_VERSION_MINOR, ELEMD_VERSION_PATCH);
        applicationInfo.apiVersion = VK_API_VERSION_1_2;


        // --------------- Get Required Instance Extensions ---------------

        std::vector<const char*> extensions = {};
        uint32_t extensionCount = 0;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        for (uint32_t i = 0; i < extensionCount; ++i)
        {
            extensions.push_back(requiredExtensions[i]);
        }


        // --------------- Create Instance Create Info ---------------

        const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = (uint32_t)validationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();


        // --------------- Create Instance ---------------

        vku::err_check(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
    }

    void VulkanContext::create_surface()
    {
        // --------------- Create WIndow Surface ---------------

        vku::err_check(glfwCreateWindowSurface(instance, _window->getGLFWWindow(),
                                               nullptr, &surface));
    }

    void VulkanContext::create_physical_devices()
    {
        // --------------- Get Physical Devices ---------------

        vku::err_check(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        if (physicalDeviceCount == 0)
        {
            vku::err("Could not find a suitable Device that supports Vulkan!");
            exit(EXIT_FAILURE);
        }

        physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
        vku::err_check(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices));
        
        
        // --------------- Select Best Physical Device ---------------

        bestDevice = vku::select_physical_device(physicalDevices, physicalDeviceCount);
    }

    void VulkanContext::check_surface_support()
    {
        // --------------- Check Surface Support ---------------

        VkBool32 surfaceSupport = false;
        vku::err_check(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[bestDevice.deviceIndex],
                                                            bestDevice.queueFamilyIndex, surface,
                                                            &surfaceSupport));
        if (!surfaceSupport)
        {
            std::cerr << "Error: Swapchain not supported on your Device!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void VulkanContext::create_logical_device()
    {
        // --------------- Create Device Queue Create Info ---------------

        float queuePriorities[] = {1.0f, 1.0f, 1.0f, 1.0f};

        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = bestDevice.queueFamilyIndex;
        deviceQueueCreateInfo.queueCount = bestDevice.queueCount;
        deviceQueueCreateInfo.pQueuePriorities = queuePriorities;


        // --------------- Create Device Create Info ---------------

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

        const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;


        // --------------- Create Device ---------------

        vku::err_check(
            vkCreateDevice(physicalDevices[bestDevice.deviceIndex], &deviceCreateInfo, nullptr, &device));
    }

    void VulkanContext::load_vulkan()
    {
        // --------------- Load Vulkan ---------------

        if (gladLoaderLoadVulkan(instance, physicalDevices[bestDevice.deviceIndex], device) <= 0)
        {
            vku::err("Could not load the Vulkan device!");
            exit(EXIT_FAILURE);
        }
    }

    void VulkanContext::create_queue()
    {
        // --------------- Get Device Queue ---------------

        vkGetDeviceQueue(device, bestDevice.queueFamilyIndex, 0, &queue);
    }

    void VulkanContext::create_swapchain()
    {
        // --------------- Get Surface Capabilities ---------------

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vku::err_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevices[bestDevice.deviceIndex], surface, &surfaceCapabilities));


        // --------------- Get Surface Formats ---------------

        uint32_t surfaceFormatCount = 0;
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[bestDevice.deviceIndex],
                                                            surface, &surfaceFormatCount, nullptr));
        VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
        vku::err_check(vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevices[bestDevice.deviceIndex], surface, &surfaceFormatCount, surfaceFormats));


        // --------------- Select Image Count ---------------

        uint32_t selectedImageCount = surfaceCapabilities.minImageCount + 1;
        if (selectedImageCount > surfaceCapabilities.maxImageCount)
            selectedImageCount = surfaceCapabilities.maxImageCount;


        // --------------- Select Image Format and Colorspace ---------------

        selectedImageFormat = surfaceFormats[0].format;
        VkColorSpaceKHR selectedImageColorSpace = surfaceFormats[0].colorSpace;
        for (uint32_t i = 0; i < surfaceFormatCount; ++i)
        {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
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
            physicalDevices[bestDevice.deviceIndex], surface, &presentModeCount, nullptr));
        VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
        vku::err_check(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices[bestDevice.deviceIndex], surface, &presentModeCount, presentModes));


        // --------------- Select Present Mode ---------------
            
        VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (uint32_t i = 0; i < presentModeCount; ++i)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                selectedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }


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
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;


        // --------------- Create Swapchain ---------------

        vku::err_check(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain));


        // --------------- Cleanup ---------------

        delete[] presentModes;
        delete[] surfaceFormats;
    }

    void VulkanContext::create_image_views()
    {
        // --------------- Get Swapchain Image ---------------

        vku::err_check(
            vkGetSwapchainImagesKHR(device, swapchain, &actualSwapchainImageCount, nullptr));
        VkImage* swapchainImages = new VkImage[actualSwapchainImageCount];
        vku::err_check(vkGetSwapchainImagesKHR(device, swapchain, &actualSwapchainImageCount,
                                               swapchainImages));

        
        // --------------- Create Image Views ---------------

        imageViews = new VkImageView[actualSwapchainImageCount];
        for (uint32_t i = 0; i < actualSwapchainImageCount; ++i)
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
                vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageViews[i]));
        }

        
        // --------------- Cleanup ---------------

        delete[] swapchainImages;
    }

    void VulkanContext::create_render_pass()
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

        vku::err_check(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass));
    }

    void VulkanContext::create_pipeline()
    {
        // --------------- Load Shader ---------------

        fragShaderModule = new VkShaderModule();
        vertShaderModule = new VkShaderModule();
        create_shader_module("../../res/shader/shader.frag.spv", fragShaderModule);
        create_shader_module("../../res/shader/shader.vert.spv", vertShaderModule);


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

        
        // --------------- Create Pipeline Vertex Input State Create Info ---------------

        VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
        pipelineVertexInputStateCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pipelineVertexInputStateCreateInfo.pNext = nullptr;
        pipelineVertexInputStateCreateInfo.flags = 0;
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


        // --------------- Create Pipeline Layout Create Info ---------------

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pNext = nullptr;
        pipelineLayoutCreateInfo.flags = 0;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;


        // --------------- Create Pipeline Layout ---------------

        vku::err_check(
            vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));


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
        graphicsPipelineCreateInfo.pDynamicState = nullptr;
        graphicsPipelineCreateInfo.layout = pipelineLayout;
        graphicsPipelineCreateInfo.renderPass = renderPass;
        graphicsPipelineCreateInfo.subpass = 0;
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        graphicsPipelineCreateInfo.basePipelineIndex = -1;


        // --------------- Create Graphics Pipeline ---------------

        vku::err_check(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                                 &graphicsPipelineCreateInfo, nullptr, &pipeline));
    }

    void VulkanContext::create_framebuffer()
    {
        // --------------- Create Framebuffers ---------------

        frameBuffers = new VkFramebuffer[actualSwapchainImageCount];

        for (uint32_t i = 0; i < actualSwapchainImageCount; ++i)
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
                vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &(frameBuffers[i])));
        }
    }

    void VulkanContext::create_command_pool()
    {
        // --------------- Create Command Pool Create Info ---------------

        VkCommandPoolCreateInfo commandPoolCreateInfo;
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;
        commandPoolCreateInfo.flags = 0;
        commandPoolCreateInfo.queueFamilyIndex = bestDevice.queueFamilyIndex;


        // --------------- Create Command Pool ---------------

        vku::err_check(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool));
    }

    void VulkanContext::create_command_buffers()
    {
        // --------------- Create Command Buffer Allocate Info ---------------

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = nullptr;
        commandBufferAllocateInfo.commandPool = commandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = actualSwapchainImageCount;


        // --------------- Allocate Command Buffers ---------------

        commandBuffers = new VkCommandBuffer[actualSwapchainImageCount];
        vku::err_check(
            vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers));
    }

    void VulkanContext::record_command_buffers()
    {
        // --------------- Create Command Buffer Begin Info ---------------

        VkCommandBufferBeginInfo commandBufferBeginInfo;
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext = nullptr;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        for (uint32_t i = 0; i < actualSwapchainImageCount; ++i)
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

            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            vku::err_check(vkEndCommandBuffer(commandBuffers[i]));
        }
    }

    void VulkanContext::create_semaphores()
    {
        // --------------- Create Semaphore Create Info ---------------

        VkSemaphoreCreateInfo semaphoreCreateInfo;
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;


        // --------------- Create Semaphores ---------------

        vku::err_check(
            vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphoreImageAvailable));
        vku::err_check(
            vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphoreRenderingComplete));
    }

    VulkanContext::VulkanContext(Window* window)
    {
        _window = (WindowImpl*)window;

        width = (uint32_t)_window->getWidth();
        height = (uint32_t)_window->getHeight();        

        preload_vulkan();
        create_instance();
        create_surface();
        create_physical_devices();
        check_surface_support();
        create_logical_device();
        load_vulkan();
        create_queue();
        create_swapchain();
        create_image_views();
        create_render_pass();
        create_pipeline();
        create_framebuffer();
        create_command_pool();
        create_command_buffers();
        record_command_buffers();
        create_semaphores();

#ifndef NDEBUG
        std::cout << "{\n";
        vku::print_layers();
        vku::print_extensions();
        vku::print_physical_devices(physicalDevices, physicalDeviceCount);
        vku::print_selected_device(bestDevice);
        std::cout << "}\n";
#endif
    }

    VulkanContext::~VulkanContext()
    {
        vkDeviceWaitIdle(device);

        vkDestroySemaphore(device, semaphoreImageAvailable, nullptr);
        vkDestroySemaphore(device, semaphoreRenderingComplete, nullptr);
        vkFreeCommandBuffers(device, commandPool, actualSwapchainImageCount,
                             commandBuffers);
        vkDestroyCommandPool(device, commandPool, nullptr);
        for (uint32_t i = 0; i < actualSwapchainImageCount; ++i)
        {
          vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
        }
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyShaderModule(device, *vertShaderModule, nullptr);
        vkDestroyShaderModule(device, *fragShaderModule, nullptr);
        for (uint32_t i = 0; i < actualSwapchainImageCount; ++i)
        {
            vkDestroyImageView(device, imageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        
        delete[] commandBuffers;
        delete[] frameBuffers;
        delete vertShaderModule;
        delete fragShaderModule;
        delete[] imageViews;
        delete[] physicalDevices;
    }

    void VulkanContext::create_shader_module(const std::string& filename,
                                           VkShaderModule* shaderModule)
    {
        std::vector<char> spirvCode = read_shader(filename);

        VkShaderModuleCreateInfo shaderModuleCreateInfo;
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.pNext = nullptr;
        shaderModuleCreateInfo.flags = 0;
        shaderModuleCreateInfo.codeSize = spirvCode.size();
        shaderModuleCreateInfo.pCode = (uint32_t*)spirvCode.data();

        vku::err_check(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, shaderModule));
    }

    std::vector<char> VulkanContext::read_shader(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (file)
        {
            size_t fileSize = (size_t)file.tellg();
            std::vector<char> fileBuffer(fileSize);
            file.seekg(0);
            file.read(fileBuffer.data(), fileSize);
            file.close();
            return fileBuffer;
        }
        
        return std::vector<char>();
    }

} // namespace elemd