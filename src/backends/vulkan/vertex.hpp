#ifndef ELEMD_VERTEX_HPP
#define ELEMD_VERTEX_HPP

#include <array>

#include "elemd/color.hpp"
#include "elemd/vec2.hpp"
#include "glad/vulkan.h"

namespace elemd
{
    class point_vertex
    {
    public:
        vec2 position;
        // vec2 uv;
        // color fill_color;
        // float border_radius;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 1> gerAttributeDescriptions();
    };

} // namespace elemd

#endif // ELEMD_VERTEX_HPP