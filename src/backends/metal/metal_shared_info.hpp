#ifndef ELEMD_METAL_SHARED_INFO_HPP
#define ELEMD_METAL_SHARED_INFO_HPP


#include <Metal/Metal.hpp>

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