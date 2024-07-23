#ifndef vk_buffer_h
#define vk_buffer_h

#include "gfx/vk/vk_device.h"
#include "gfx/vk/vk_cmd_buffers.h"
#include "gfx/vk/vk_vertex.h"

typedef struct
{
    VkDeviceMemory memory;
    VkBuffer buffer;
}   vk_buffer;

vk_buffer vk_vertex_buffer_init(vk_device *d, vk_vertex *vertices, size_t vb_size);
vk_buffer vk_index_buffer_init(vk_device *d, uint16_t *indices, size_t ib_size);
void vk_buffers_destroy(vk_device *d, vk_buffer *vb, vk_buffer *ib);

#endif
