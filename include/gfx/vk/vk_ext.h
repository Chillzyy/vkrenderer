#ifndef vk_ext_h
#define vk_ext_h

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern const uint32_t VALIDATION_LAYER_COUNT;
extern const char *VALIDATION_LAYERS[];

extern const uint32_t DEVICE_EXTENSION_COUNT;
extern const char *DEVICE_EXTENSIONS[];

typedef struct
{
    VkSurfaceCapabilitiesKHR capabilities;
    uint32_t format_count;
    VkSurfaceFormatKHR formats[8];
    uint32_t present_mode_count;
    VkPresentModeKHR present_modes[8];
}   swapchain_support_details;

bool vk_layer_support(void);
void vk_ext_init(bool debugging, const char **exts, uint32_t *ext_count);

#endif
