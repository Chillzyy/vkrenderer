#include "gfx/vk/vk_vertex.h"

VkVertexInputBindingDescription vk_vertex_binding_desc(void)
{
    VkVertexInputBindingDescription binding_desc =
    {
        .binding = 0,
        .stride = sizeof(vk_vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    return binding_desc;
}

VkVertexInputAttributeDescription vk_vertex_attrib_pos(void)
{
    VkVertexInputAttributeDescription pos_desc =
    {
        .binding = 0,
        .location = 0,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = offsetof(vk_vertex, pos)
    };

    return pos_desc;
}

VkVertexInputAttributeDescription vk_vertex_attrib_color(void)
{
    VkVertexInputAttributeDescription color_desc =
    {
        .binding = 0,
        .location = 1,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(vk_vertex, color)
    };

    return color_desc;
}
