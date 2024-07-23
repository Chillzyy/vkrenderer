#ifndef vk_instance_h
#define vk_instance_h

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "gfx/vk/vk_ext.h"
#include "gfx/vk/vk_debug.h"
#include <stdio.h>
#include <stdbool.h>

VkInstance vk_instance_init(const char *title, bool debugging);
void vk_instance_destroy(VkInstance instance);

#endif
