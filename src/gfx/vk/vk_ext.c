#include "gfx/vk/vk_ext.h"

const uint32_t VALIDATION_LAYER_COUNT = 1;
const char *VALIDATION_LAYERS[] =
{
    "VK_LAYER_KHRONOS_validation"
};

const uint32_t DEVICE_EXTENSION_COUNT = 1;
const char *DEVICE_EXTENSIONS[] =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


bool vk_layer_support(void)
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties available_layers[layer_count];
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (uint32_t i = 0; i < VALIDATION_LAYER_COUNT; i++)
    {
        bool layer_found = false;
        for (uint32_t j = 0; j < layer_count; j++)
        {
            if (strcmp(VALIDATION_LAYERS[i], available_layers[j].layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }
        if (!layer_found)
        {
            return false;
        }
    }

    return true;
}

void vk_ext_init(bool debugging, const char **exts, uint32_t *ext_count)
{
    uint32_t glfw_ext_count = 0;
    const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

    if (glfw_extensions == NULL)
    {
        printf("Failed to find required instance extensions\n");
        return;
    }

    if (exts == NULL)
    {
        *ext_count = glfw_ext_count + debugging;
        return;
    }

    for (uint32_t i = 0; i < glfw_ext_count; i++)
    {
        exts[i] = glfw_extensions[i];
    }

    if (debugging)
    {
        exts[glfw_ext_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }
}
