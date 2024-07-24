#ifndef vk_surface_h
#define vk_surface_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>

VkSurfaceKHR vk_surface_init(VkInstance instance, GLFWwindow *window);
void vk_surface_destroy(VkInstance instance, VkSurfaceKHR surface);

#endif
