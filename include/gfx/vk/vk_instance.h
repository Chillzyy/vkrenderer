#ifndef vk_instance_h
#define vk_instance_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "gfx/vk/vk_ext.h"
#include "gfx/vk/vk_debug.h"

VkInstance vk_instance_init(const char *title, bool debugging);
void vk_instance_destroy(VkInstance instance);

#endif
