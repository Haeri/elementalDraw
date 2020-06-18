#include "elemental_draw/vulkan_context.hpp"

#include <vector>

#include "vulkan_utils.hpp"
#include <GLFW/glfw3.h>

#include "elemental_draw/elemental_draw.hpp"
#include "elemental_draw/window.hpp"

VulkanContext::VulkanContext(Window* window)
{
	if (gladLoaderLoadVulkan(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) <= 0)
	{
		std::cerr << "Error: Could not load Vulkan!" << std::endl;
		exit(EXIT_FAILURE);
	}


	// --------------- Create application Info ---------------

	VkApplicationInfo applicationInfo;
	applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext				= nullptr;
	applicationInfo.pApplicationName	= "UI Application";			// TODO: Pass down from application
	applicationInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName			= ELEM_APPLICATION_NAME;
	applicationInfo.engineVersion		= VK_MAKE_VERSION(ELEM_VERSION_MAJOR, ELEM_VERSION_MINOR, ELEM_VERSION_PATCH);
	applicationInfo.apiVersion			= VK_API_VERSION_1_2;


	// --------------- Check Layers ---------------

	uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    VkLayerProperties* layerProperties = new VkLayerProperties[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);

	std::cout << "Layers: " << layerCount << std::endl;
    for (int i = 0; i < layerCount; ++i) 
	{		
		std::cout << "Name:         " << layerProperties[i].layerName << "\n"
                  << "Spec Version: " << layerProperties[i].specVersion << "\n"
                  << "Impl Version: " << layerProperties[i].implementationVersion << "\n"
				  << "Description:  " << layerProperties[i].description << "\n"
				  << "\n";
    }

	const std::vector<const char*> validationLayers = { 
		"VK_LAYER_LUNARG_standard_validation" 
	};


	// --------------- Check Extensions ---------------

	uint32_t extencionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extencionCount, nullptr);
    VkExtensionProperties* extensionProperties = new VkExtensionProperties[extencionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extencionCount,extensionProperties);

    std::cout << "Extensions: " << layerCount << std::endl;
    for (int i = 0; i < layerCount; ++i)
    {
        std::cout << "Name:         " << extensionProperties[i].extensionName << "\n"
                  << "Spec Version: " << extensionProperties[i].specVersion << "\n"
                  << "\n";
    }

	std::vector<const char*> extensions = {};
    uint32_t extensionCount = 0;
	const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    for (uint32_t i = 0; i < extensionCount; ++i)
    {
        extensions.push_back(requiredExtensions[i]);
    }
	 
	// --------------- Create Instance create info ---------------

	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext					= nullptr;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
    instanceCreateInfo.enabledLayerCount		= validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames		= validationLayers.data();
	instanceCreateInfo.enabledExtensionCount	= extensions.size();
    instanceCreateInfo.ppEnabledExtensionNames	= extensions.data();


	// --------------- Create Instance ---------------

	_vulkanInstance = new VkInstance();
	vku::err_check(vkCreateInstance(&instanceCreateInfo, nullptr, _vulkanInstance));


	// --------------- Create Surface ---------------

	_vulkanSurface = new VkSurfaceKHR();
    vku::err_check(glfwCreateWindowSurface(*_vulkanInstance, window->getWindow(), nullptr,
                                          _vulkanSurface));
	
	

	// --------------- Cet devices ---------------

	uint32_t physicalDeviceCount = 0;
	vku::err_check(vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, nullptr));

	if (physicalDeviceCount == 0)
    {
        std::cerr << "Error: Could not find a suitable Device that supports Vulkan!" << std::endl;
        exit(EXIT_FAILURE);	
	}

	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
	vku::err_check(vkEnumeratePhysicalDevices(*_vulkanInstance, &physicalDeviceCount, physicalDevices));

	for (uint32_t i = 0; i < physicalDeviceCount; ++i) 
	{
		vku::print_device(physicalDevices[i]);
	}


	// --------------- Decide on best device ---------------

	vku::BestDevice bestDevice = vku::get_best_device_info(physicalDevices, physicalDeviceCount);
	std::cout
		<< "Best Device Nr. " << (bestDevice.deviceIndex+1) << "\n"
		<< "Best Queue Family: " << (bestDevice.queueFamilyIndex+1) << " with queue count: " << bestDevice.queueCount << "\n";


	// --------------- Create Queue ---------------

	float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.pNext				= nullptr;
	deviceQueueCreateInfo.flags				= 0;
	deviceQueueCreateInfo.queueFamilyIndex	= bestDevice.queueFamilyIndex;
	deviceQueueCreateInfo.queueCount		= bestDevice.queueCount;
    deviceQueueCreateInfo.pQueuePriorities	= queuePriorities;


	// --------------- Create Device create info ---------------

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext						= nullptr;
	deviceCreateInfo.flags						= 0;
	deviceCreateInfo.queueCreateInfoCount		= 1;
	deviceCreateInfo.pQueueCreateInfos			= &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount			= 0;
	deviceCreateInfo.ppEnabledLayerNames		= nullptr;
	deviceCreateInfo.enabledExtensionCount		= 0;
	deviceCreateInfo.ppEnabledExtensionNames	= nullptr;
	deviceCreateInfo.pEnabledFeatures			= &physicalDeviceFeatures;


	// --------------- Create Device ---------------

	_VulkanDevice = new VkDevice();
	vku::err_check(vkCreateDevice(physicalDevices[bestDevice.deviceIndex], &deviceCreateInfo, nullptr, _VulkanDevice));
	
	if (gladLoaderLoadVulkan(*_vulkanInstance, physicalDevices[bestDevice.deviceIndex], *_VulkanDevice) <= 0) {
		std::cerr << "Error: Could not load the Vulkan device!" << std::endl;
		exit(EXIT_FAILURE);
	}


	// --------------- Create Queue ---------------

	VkQueue queue;
    vkGetDeviceQueue(*_VulkanDevice, bestDevice.queueFamilyIndex, 0, &queue);


	delete[] physicalDevices;
    delete[] extensionProperties;
	delete[] layerProperties;
}

VulkanContext::~VulkanContext()
{

	vkDeviceWaitIdle(*_VulkanDevice);

	//vkDestroySurfaceKHR(*_vulkanInstance, *_vulkanSurface, nullptr);
	vkDestroyDevice(*_VulkanDevice, nullptr);
    vkDestroyInstance(*_vulkanInstance, nullptr);

	//delete _vulkanSurface;
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