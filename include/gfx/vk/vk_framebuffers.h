#ifndef vk_framebuffers_h
#define vk_framebuffers_h

#include "gfx/vk/vk_device.h"

VkResult vk_framebuffers_init(vk_device *d, VkRenderPass render_pass, VkExtent2D img_extent, VkImageView *img_views, VkFramebuffer *framebuffers, uint32_t img_count);
void vk_framebuffers_destroy(vk_device *d, VkFramebuffer *framebuffers, uint32_t img_count);

#endif
