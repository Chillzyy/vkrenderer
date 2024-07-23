#include "gfx/vk/vk_surface.h"

VkSurfaceKHR vk_surface_init(VkInstance instance, GLFWwindow *window)
{
    VkResult result = VK_SUCCESS;
    VkSurfaceKHR surface = {0};

    result = glfwCreateWindowSurface(instance, window, NULL, &surface);

    if (result != VK_SUCCESS)
    {
        printf("Failed to create window surface\n");
    }

    return surface;
}

void vk_surface_destroy(VkInstance instance, VkSurfaceKHR surface)
{
    vkDestroySurfaceKHR(instance, surface, NULL);
}
