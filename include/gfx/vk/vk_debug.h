#ifndef vk_debug_h
#define vk_debug_h

#include <stdio.h>
#include <inttypes.h>
#include "vulkan/vulkan.h"

VkDebugUtilsMessengerEXT vk_debug_messenger_init(VkInstance instance);
void vk_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *debug_info);
VKAPI_ATTR VkResult VKAPI_CALL vk_debug_messenger_create(VkInstance instance, VkDebugUtilsMessengerEXT *debug_messenger, const VkDebugUtilsMessengerCreateInfoEXT *debug_info, const VkAllocationCallbacks *alloc);
VKAPI_ATTR void VKAPI_CALL vk_debug_messenger_destroy(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks *alloc);


#endif
