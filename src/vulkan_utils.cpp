#ifndef ELEM_VULKAN_UTILS_CPP
#define ELEM_VULKAN_UTILS_CPP

#include <iostream>
#include <glad/vulkan.h>

namespace vku 
{
	struct BestDevice 
	{
		int deviceIndex;
		uint32_t queueFamilyIndex;
		uint32_t queueCount;
	};

	inline void err(std::string message)
	{
		std::cerr << "VK ERROR: " << message << std::endl;
		// exit(EXIT_FAILURE);
	}

	inline std::string device_type(const VkPhysicalDeviceType& deviceType)
	{
		std::string dtype = "";

		switch (deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			dtype = "Integrate GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			dtype = "Discrete GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			dtype = "Virtual GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			dtype = "CPU";
			break;
		default:
			dtype = "Unknown";
			break;
		}

		return dtype;
	}

	inline BestDevice get_best_device_info(VkPhysicalDevice* devices, uint32_t count)
	{	
		int bestDeviceIndex = 0;
		uint32_t bestQueuFamily = 0;
		uint32_t maxQueueCount = 0;

		for (int d = 0; d < count; ++d) {
			uint32_t queueFamilyPropertyCount;
			vkGetPhysicalDeviceQueueFamilyProperties(devices[d], &queueFamilyPropertyCount, NULL);

			VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[queueFamilyPropertyCount];
			vkGetPhysicalDeviceQueueFamilyProperties(devices[d], &queueFamilyPropertyCount, queueFamilyProperties);

			for (uint32_t f = 0; f < queueFamilyPropertyCount; ++f)
			{
				if (maxQueueCount < queueFamilyProperties[f].queueCount)
				{
					maxQueueCount = queueFamilyProperties[f].queueCount;
					bestQueuFamily = f;
					bestDeviceIndex = d;
				}
			}

			delete[] queueFamilyProperties;
		}

		return BestDevice{ bestDeviceIndex, bestQueuFamily, maxQueueCount};
	}

	inline void print_device(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		std::cout
			<< "GPU Device: " << properties.deviceName << "\n"
			<< "\tAPI Version: " << VK_VERSION_MAJOR(properties.apiVersion) << "." << VK_VERSION_MINOR(properties.apiVersion) << "." << VK_VERSION_PATCH(properties.apiVersion) << "\n"
			<< "\tDriver Version: " << properties.driverVersion << "\n"
			<< "\tVendor ID: " << properties.vendorID << "\n"
			<< "\tDevice ID: " << properties.deviceID << "\n"
			<< "\tDevice Type: " << device_type(properties.deviceType) << "\n" ;

		
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		std::cout << "\tFeatureset:\n"
			<< "\t\tGeometry shader:\t" << features.geometryShader << "\n"
			<< "\t\tTesselation shader:\t" << features.tessellationShader << "\n";


		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &physicalDeviceMemoryProperties);


		uint32_t queueFamilyPropertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertyCount, NULL);

		VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[queueFamilyPropertyCount];
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyPropertyCount, queueFamilyProperties);

		for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
		{
			std::cout
				<< "\tQueue Family Nr. " << (i+1) << "\n"
				<< "\t\tGraphics bit:\t\t" << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) << "\n"
				<< "\t\tCompute bit:\t\t" << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << "\n"
				<< "\t\tTransfer bit:\t\t" << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << "\n"
				<< "\t\tSparse binding bit:\t" << ((queueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << "\n"
				<< "\t\tQueue count:\t\t" << queueFamilyProperties[i].queueCount << "\n"
				<< "\t\tTimestamp bits:\t\t" << queueFamilyProperties[i].timestampValidBits << "\n";
		}

		delete[] queueFamilyProperties;
	}

	inline bool err_check(const VkResult& result)
	{
		if (result != VK_SUCCESS)
		{
			switch (result) {
			case VK_NOT_READY:
				err("A fence or query has not yet completed");
				break;
			case VK_TIMEOUT:
				err("A wait operation has not completed in the specified time");
				break;
			case VK_EVENT_SET:
				err("An event is signaled");
				break;
			case VK_EVENT_RESET:
				err("An event is unsignaled");
				break;
			case VK_INCOMPLETE:
				err("A return array was too small for the result");
				break;
			//case VK_SUBOPTIMAL_KHR:
			//	err("A swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully.");
			//	break;
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				err("A host memory allocation has failed.");
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				err("A device memory allocation has failed.");
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				err("Initialization of an object could not be completed for implementation-specific reasons.");
				break;
			case VK_ERROR_DEVICE_LOST:
				err("The logical or physical device has been lost. See Lost Device");
				break;
			case VK_ERROR_MEMORY_MAP_FAILED:
				err("Mapping of a memory object has failed.");
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				err("A requested layer is not present or could not be loaded.");
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				err("A requested extension is not supported.");
				break;
			case VK_ERROR_FEATURE_NOT_PRESENT:
				err("A requested feature is not supported.");
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				err("The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons.");
				break;
			case VK_ERROR_TOO_MANY_OBJECTS:
				err("Too many objects of the type have already been created.");
				break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				err("A requested format is not supported on this device.");
				break;
			case VK_ERROR_FRAGMENTED_POOL:
				err("A pool allocation has failed due to fragmentation of the pool’s memory. This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation. This should be returned in preference to VK_ERROR_OUT_OF_POOL_MEMORY, but only if the implementation is certain that the pool allocation failure was due to fragmentation.");
				break;
			//case VK_ERROR_SURFACE_LOST_KHR:
			//	err("A surface is no longer available.");
			//	break;
			//case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			//	err("The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again.");
			//	break;
			//case VK_ERROR_OUT_OF_DATE_KHR:
			//	err("A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail. Applications must query the new surface properties and recreate their swapchain if they wish to continue presenting to the surface.");
			//	break;
			//case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			//	err("The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.");
			//	break;
			//case VK_ERROR_INVALID_SHADER_NV:
			//	err("One or more shaders failed to compile or link. More details are reported back to the application via VK_EXT_debug_report if enabled.");
			//	break;
			case VK_ERROR_OUT_OF_POOL_MEMORY:
				err("A pool memory allocation has failed. This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation. If the failure was definitely due to fragmentation of the pool, VK_ERROR_FRAGMENTED_POOL should be returned instead.");
				break;
			case VK_ERROR_INVALID_EXTERNAL_HANDLE:
				err("An external handle is not a valid handle of the specified type.");
				break;
			case VK_ERROR_FRAGMENTATION:
				err("A descriptor pool creation has failed due to fragmentation.");
				break;
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
				err("A buffer creation or memory allocation failed because the requested address is not available. A shader group handle assignment failed because the requested shader group handle information is no longer valid.");
				break;
			//case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			//	err("An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exlusive full-screen access. This may occur due to implementation-dependent reasons, outside of the application’s control.");
			//	break;
			default:
				err("An unknown error has occurred; either the application has provided invalid input, or an implementation failure has occurred");
				break;
			}

			return false;
		}
		else {
			return true;
		}
	}
}

#endif // ELEM_VULKAN_UTILS_CPP