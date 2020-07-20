#ifndef ELEMD_VERTEX_HPP
#define ELEMD_VERTEX_HPP

#include <array>
#include <glad/vulkan.h>

#include "elemd/vec2.hpp"
#include "elemd/color.hpp"

namespace elemd
{
    class vertex
    {
    public:
        vec2 pos;
        vec2 uv;
        color col;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> gerAttributeDescriptions();
    };

} // namespace elemd

#endif // ELEMD_VERTEX_HPP