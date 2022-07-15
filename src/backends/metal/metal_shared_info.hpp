#ifndef ELEMD_METAL_SHARED_INFO_HPP
#define ELEMD_METAL_SHARED_INFO_HPP

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <string>


namespace elemd
{
    class MetalSharedInfo
    {
    public:
        static int MAX_UNIFORM_BLOCK_SIZE;

        static MetalSharedInfo* getInstance();
        static void destroy();

    private:
        static MetalSharedInfo* _instance;

        MetalSharedInfo();
        MetalSharedInfo(const MetalSharedInfo&)
        {}
        ~MetalSharedInfo();

        void load_metal();
    };

} // namespace elemd

#endif // ELEMD_METAL_SHARED_INFO_HPP