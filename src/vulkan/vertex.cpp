#include "vertex.hpp"

namespace elemd
{
    VkVertexInputBindingDescription vertex::getBindingDescription()
    {
        VkVertexInputBindingDescription vertexInputBindingDescription;
        vertexInputBindingDescription.binding = 0;
        vertexInputBindingDescription.stride = sizeof(vertex);
        vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return vertexInputBindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 2> vertex::gerAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(vertex, position);

        //attributeDescriptions[1].location = 1;
        //attributeDescriptions[1].binding = 0;
        //attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        //attributeDescriptions[1].offset = offsetof(vertex, uv);

        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(vertex, fill_color);

        //attributeDescriptions[3].location = 3;
        //attributeDescriptions[3].binding = 0;
        //attributeDescriptions[3].format = VK_FORMAT_R32_SFLOAT;
        //attributeDescriptions[3].offset = offsetof(vertex, border_radius);

        return attributeDescriptions;
    }
} // namespace elemd