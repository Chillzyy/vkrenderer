#ifndef vk_shaders_h
#define vk_shaders_h

#include "gfx/vk/vk_device.h"

typedef struct
{
    const char *vertex_shader_path;
    const char *fragment_shader_path;
    const char *entry_name;

    VkShaderModule vertex_module;
    VkShaderModule fragment_module;
}   vk_shaders;

vk_shaders vk_shaders_init(vk_device *d);

void vk_shaders_destroy(vk_device* d, vk_shaders *s);

#endif
