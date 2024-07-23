#ifndef vk_vertex_h
#define vk_vertex_h

#include "vulkan/vulkan.h"
#include "cglm/cglm.h"

typedef struct
{
    vec2 pos;
    vec3 color;
}   vk_vertex;

VkVertexInputBindingDescription vk_vertex_binding_desc(void);
VkVertexInputAttributeDescription vk_vertex_attrib_pos(void);
VkVertexInputAttributeDescription vk_vertex_attrib_color(void);

#endif
