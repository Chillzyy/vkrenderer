#ifndef vk_img_views_h
#define vk_img_views_h

#include "gfx/vk/vk_device.h"

VkResult vk_img_views_init(vk_device *d, VkImage *imgs, uint32_t img_count, VkFormat img_format, VkImageView *img_views);
void vk_img_views_destroy(vk_device *d, VkImageView *img_views, uint32_t img_count);

#endif
