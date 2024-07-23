#include "gfx/vk/vk_instance.h"

static VkApplicationInfo vk_app_info_init(const char *title)
{
    VkApplicationInfo app_info =
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = title,
        .applicationVersion = 0,
        .pEngineName = NULL,
        .engineVersion = 0,
        .apiVersion = VK_API_VERSION_1_3
    };

    return app_info;
}

static VkInstanceCreateInfo vk_instance_info_init(bool debugging, VkApplicationInfo *app_info, const char *const *extensions, const uint32_t extension_count, const char **layers, const uint32_t layer_count)
{
    VkInstanceCreateInfo instance_info =
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .flags = 0,
        .pApplicationInfo = app_info,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions
    };

    static VkDebugUtilsMessengerCreateInfoEXT debug_info = {0};
    if (debugging)
    {
        instance_info.enabledLayerCount = layer_count;
        instance_info.ppEnabledLayerNames = layers;
        vk_debug_messenger_create_info(&debug_info);
        instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debug_info;
    }
    else
    {
        instance_info.pNext = NULL;
        instance_info.enabledLayerCount = 0;
        instance_info.ppEnabledLayerNames = NULL;

    }

    return instance_info;
}

VkInstance vk_instance_init(const char *title, bool debugging)
{
    VkResult result = VK_SUCCESS;
    VkInstance instance = {0};

    if (debugging && !vk_layer_support())
    {
        printf("Validation layers requested, but not available\n");
    }

    uint32_t glfw_extension_count = 0;
    vk_ext_init(debugging, NULL, &glfw_extension_count);

    const char *glfw_extensions[glfw_extension_count];
    vk_ext_init(debugging, glfw_extensions, &glfw_extension_count);

    if (debugging)
    {
        uint32_t available_extension_count = 0;
        vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, NULL);

        VkExtensionProperties available_extensions[available_extension_count];
        vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, available_extensions);

        if (available_extension_count == 0)
        {
            printf("Debugging: Failed to find any available Vulkan extensions\n");
        }

        printf("%u available extensions: \n", available_extension_count);

        for (uint32_t i = 0; i < available_extension_count; i++)
        {
            printf("\t %s (spec version: %u)\n", available_extensions[i].extensionName, available_extensions[i].specVersion);
        }

    }

    VkApplicationInfo app_info = vk_app_info_init(title);

    VkInstanceCreateInfo instance_info = vk_instance_info_init(debugging, &app_info, glfw_extensions, glfw_extension_count, VALIDATION_LAYERS, VALIDATION_LAYER_COUNT);

    result = vkCreateInstance(&instance_info, NULL, &instance);

    if (result != VK_SUCCESS)
    {
        printf("Failed to create instance\n");
    }

    return instance;
}

void vk_instance_destroy(VkInstance instance)
{
    vkDestroyInstance(instance, NULL);
}
