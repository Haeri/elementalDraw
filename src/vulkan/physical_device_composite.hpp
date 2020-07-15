#ifndef ELEMD_PHYSICAL_DEVICE_COMPOSITE_HPP
#define ELEMD_PHYSICAL_DEVICE_COMPOSITE_HPP

#include <glad/vulkan.h>

namespace elemd
{
    class PhysicalDeviceComposite
    {
    public:
        int deviceIndex;
        uint32_t queueFamilyIndex;
        uint32_t queueCount;
    };
}

#endif // ELEMD_PHYSICAL_DEVICE_COMPOSITE_HPP