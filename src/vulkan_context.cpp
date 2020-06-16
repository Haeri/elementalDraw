#include "elemental_draw/vulkan_context.hpp"

#include <iostream>

#include "elemental_draw/elemental_draw.hpp"
#include "vulkan_utils.cpp"

VulkanContext::VulkanContext()
{
	if (gladLoaderLoadVulkan(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) <= 0)
	{
		std::cerr << "Could not load Vulkan" << std::endl;
		exit(EXIT_FAILURE);
	}


	VkApplicationInfo applicationInfo;
	applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext				= NULL;
	applicationInfo.pApplicationName	= "UI Application";			// TODO: Pass down from application
	applicationInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName			= ELEM_APPLICATION_NAME;
	applicationInfo.engineVersion		= VK_MAKE_VERSION(ELEM_VERSION_MAJOR, ELEM_VERSION_MINOR, ELEM_VERSION_PATCH);
	applicationInfo.apiVersion			= VK_API_VERSION_1_2;


	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext					= NULL;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= 0;
	instanceCreateInfo.ppEnabledLayerNames		= NULL;
	instanceCreateInfo.enabledExtensionCount	= 0;
	instanceCreateInfo.ppEnabledExtensionNames	= NULL;


	_vulkanInstance = new VkInstance();
	vku::err_check(vkCreateInstance(&instanceCreateInfo, NULL, _vulkanInstance));

	uint32_t physicalDeviceCount = 0;
	vku::err_check(vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, NULL));

	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
	vku::err_check(vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, physicalDevices));

	for (uint32_t i = 0; i < physicalDeviceCount; ++i) 
	{
		vku::print_device(physicalDevices[i]);
	}

	vku::BestDevice bestDevice = vku::get_best_device_info(physicalDevices, physicalDeviceCount);
	std::cout
		<< "Chosen Device Nr. " << (bestDevice.deviceIndex+1) << "\n"
		<< "Chosen Queue Family: " << (bestDevice.queueFamilyIndex+1) << " with queue count: " << bestDevice.queueCount << "\n";


	VkDeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.pNext				= NULL;
	deviceQueueCreateInfo.flags				= 0;
	deviceQueueCreateInfo.queueFamilyIndex	= bestDevice.queueFamilyIndex;
	deviceQueueCreateInfo.queueCount		= bestDevice.queueCount;
	deviceQueueCreateInfo.pQueuePriorities	= NULL;


	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};


	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext						= NULL;
	deviceCreateInfo.flags						= 0;
	deviceCreateInfo.queueCreateInfoCount		= 1;
	deviceCreateInfo.pQueueCreateInfos			= &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount			= 0;
	deviceCreateInfo.ppEnabledLayerNames		= NULL;
	deviceCreateInfo.enabledExtensionCount		= 0;
	deviceCreateInfo.ppEnabledExtensionNames	= NULL;
	deviceCreateInfo.pEnabledFeatures			= &physicalDeviceFeatures;


	_VulkanDevice = new VkDevice();
	vku::err_check(vkCreateDevice(physicalDevices[bestDevice.deviceIndex], &deviceCreateInfo, NULL, _VulkanDevice));
	
	if (gladLoaderLoadVulkan(*_vulkanInstance, physicalDevices[bestDevice.deviceIndex], *_VulkanDevice) <= 0) {
		std::cerr << "Could not load the Vulkan device!" << std::endl;
		exit(EXIT_FAILURE);
	}

	delete[] physicalDevices;
}

VulkanContext::~VulkanContext()
{
	delete _VulkanDevice;
	delete _vulkanInstance;
}

Context* VulkanContext::create_context()
{
	return nullptr;
}

void VulkanContext::delete_context()
{
}

void VulkanContext::stroke_rect(float x, float y, float width, float height)
{
}

void VulkanContext::stroke_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl)
{
}

void VulkanContext::stroke_line(float x, float y)
{
}

void VulkanContext::stroke_circle(float x, float y, float radius)
{
}

void VulkanContext::stroke_ellipse(float x, float y, float width, float height)
{
}

void VulkanContext::stroke_polygon(float x, float y)
{
}

void VulkanContext::fill_rect(float x, float y, float width, float height)
{
}

void VulkanContext::fill_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl)
{
}

void VulkanContext::fill_circle(float x, float y, float radius)
{
}

void VulkanContext::fill_ellipse(float x, float y, float width, float height)
{
}

void VulkanContext::fill_polygon(float x, float y)
{
}

void VulkanContext::draw_text(float x, float y, char* text)
{
}

void VulkanContext::draw_image(float x, float y, float width, float height, uint32_t image)
{
}

void VulkanContext::draw_rounded_image(float x, float y, float width, float height, uint32_t image, float tl, float tr, float br, float bl)
{
}

void VulkanContext::set_color(Color color)
{
}

void VulkanContext::set_line_width(float width)
{
}

void VulkanContext::clear()
{
}

void VulkanContext::clear_rect(float x, float y, float width, float height)
{
}