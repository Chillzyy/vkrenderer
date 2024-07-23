#ifndef vk_render_pass_h
#define vk_render_pass_h

#include "gfx/vk/vk_swapchain.h"

VkRenderPass vk_render_pass_init(vk_device *d, vk_swapchain *s);
void vk_render_pass_destroy(vk_device *d, VkRenderPass render_pass);

#endif
