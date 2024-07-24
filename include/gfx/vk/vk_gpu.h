#ifndef vk_gpu_h
#define vk_gpu_h

#include "gfx/vk/vk_queue.h"
#include "gfx/vk/vk_ext.h"

swapchain_support_details vk_swapchain_support_init(VkPhysicalDevice gpu, VkSurfaceKHR surface);
VkPhysicalDevice vk_gpu_init(VkInstance instance, VkSurfaceKHR surface);

#endif
