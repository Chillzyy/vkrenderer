#ifndef vk_instance_h
#define vk_instance_h

#include "gfx/vk/vk_ext.h"
#include "gfx/vk/vk_debug.h"

VkInstance vk_instance_init(const char *title, bool debugging);
void vk_instance_destroy(VkInstance instance);

#endif
