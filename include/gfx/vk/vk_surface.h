#ifndef vk_surface_h
#define vk_surface_h

#include <stdio.h>
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

VkSurfaceKHR vk_surface_init(VkInstance instance, GLFWwindow *window);
void vk_surface_destroy(VkInstance instance, VkSurfaceKHR surface);

#endif
