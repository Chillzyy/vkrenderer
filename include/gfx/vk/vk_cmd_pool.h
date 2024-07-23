#ifndef vk_cmd_pool_h
#define vk_cmd_pool_h

#include <stdio.h>
#include "vulkan/vulkan.h"

VkCommandPool vk_cmd_pool_init(VkDevice device, VkCommandPoolCreateFlags flags, uint32_t queue_family_index);
void vk_cmd_pool_destroy(VkDevice device, VkCommandPool cmd_pool);

#endif
