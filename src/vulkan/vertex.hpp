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
        vec2 position;
        vec2 uv;
        color background_color;
        float border_radius[4];

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 4> gerAttributeDescriptions();
    };

} // namespace elemd

#endif // ELEMD_VERTEX_HPP