#ifndef vk_device_h
#define vk_device_h

#include "gfx/vk/vk_window.h"
#include "gfx/vk/vk_surface.h"
#include "gfx/vk/vk_instance.h"
#include "gfx/vk/vk_debug.h"
#include "gfx/vk/vk_gpu.h"
#include "gfx/vk/vk_cmd_pool.h"

typedef struct
{
    GLFWwindow *window;
    VkSurfaceKHR surface;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice gpu;
    VkCommandPool graphics_cmd_pool;    // One command pool for submitting draw commands
    VkCommandPool transfer_cmd_pool;    // Another for uploading vertices to GPU
    queue_family_indices indices;
    vk_queues queues;
    VkDevice device;
}   vk_device;

vk_device vk_device_init(const char *title, bool debugging);
void vk_device_destroy(vk_device *d, bool debugging);

#endif
