#include "gfx/vk/vk_debug.h"

VkDebugUtilsMessengerEXT vk_debug_messenger_init(VkInstance instance)
{
    VkResult result = VK_SUCCESS;
    VkDebugUtilsMessengerEXT debug_messenger = {0};
    VkDebugUtilsMessengerCreateInfoEXT debug_info = {0};
    vk_debug_messenger_create_info_init(&debug_info);

    result = vk_debug_messenger_create(instance, &debug_messenger, &debug_info, NULL);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create debug messenger\n");
    }

    return debug_messenger;
}

static const char *vk_debug_severity(VkDebugUtilsMessageSeverityFlagBitsEXT msg_sev)
{
    switch (msg_sev)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            return "Verbose";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            return "Info";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            return "Warning";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            return "Error";
        default:
            printf("Invalid severity code: %d\n", msg_sev);
    }

    return "No such severity\n";
}

static const char *vk_debug_type(VkDebugUtilsMessageTypeFlagBitsEXT msg_type)
{
    switch (msg_type)
    {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            return "General";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            return "Validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            return "Performance";
        default:
            printf("Invalid type code: %d\n", msg_type);
    }

    return "No such type\n";
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT msg_sev, VkDebugUtilsMessageTypeFlagBitsEXT msg_type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    printf("Validation Layer: %s\n", callback_data->pMessage);
    printf("\t Severity: %s\n", vk_debug_severity(msg_sev));
    printf("\t Type: %s\n", vk_debug_type(msg_type));
    printf("\t Objects: ");

    for (uint32_t i = 0; i < callback_data->objectCount; i++)
    {
        printf("%lx\n", callback_data->pObjects[i].objectHandle);
    }

    // This does nothing but this can't be removed due to unused parameter
    printf("\t Optional user data: %p\n", user_data);

    return VK_FALSE;
}

void vk_debug_messenger_create_info_init(VkDebugUtilsMessengerCreateInfoEXT *debug_info)
{
    debug_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info->pNext = NULL;
    debug_info->flags = 0;
    debug_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info->pfnUserCallback = vk_debug_callback;
    debug_info->pUserData = NULL;
}

VKAPI_ATTR VkResult VKAPI_CALL vk_debug_messenger_create(VkInstance instance, VkDebugUtilsMessengerEXT *debug_messenger, const VkDebugUtilsMessengerCreateInfoEXT *debug_info, const VkAllocationCallbacks *alloc)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL)
    {
        return func(instance, debug_info, alloc, debug_messenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VKAPI_ATTR void VKAPI_CALL vk_debug_messenger_destroy(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks *alloc)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL)
    {
        func(instance, debug_messenger, alloc);
    }
}
