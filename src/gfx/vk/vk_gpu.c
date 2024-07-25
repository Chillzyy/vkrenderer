#include "gfx/vk/vk_gpu.h"

static bool vk_gpu_extension_support(VkPhysicalDevice gpu)
{
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(gpu, NULL, &extension_count, NULL);

    VkExtensionProperties available_extensions[extension_count];
    vkEnumerateDeviceExtensionProperties(gpu, NULL, &extension_count, available_extensions);

    for (uint32_t i = 0; i < DEVICE_EXTENSION_COUNT; i++)
    {
        bool extension_found = false;
        for (uint32_t j = 0; j < extension_count; j++)
        {
            if (strcmp(DEVICE_EXTENSIONS[i], available_extensions[j].extensionName) == 0)
            {
                extension_found = true;
                break;
            }
        }
        if (!extension_found)
        {
            return false;
        }
    }

    return true;
}

static bool vk_gpu_suitability(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
    queue_family_indices indices = vk_queue_families_init(gpu, surface);

    bool suitable = indices.graphics.found && indices.present.found && indices.transfer.found;
    suitable &= vk_gpu_extension_support(gpu);

    if (suitable)
    {
        swapchain_support_details swapchain_support = vk_swapchain_support_init(gpu, surface);
        suitable &= (swapchain_support.present_mode_count != 0 && swapchain_support.format_count != 0);
    }

    return suitable;
}

swapchain_support_details vk_swapchain_support_init(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
    swapchain_support_details details = {0};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &details.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, NULL);

    VkSurfaceFormatKHR formats[format_count];
    if (format_count == 0)
    {
        printf("Failed to find available surface formats\n");
    }
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, formats);

    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, NULL);

    VkPresentModeKHR present_modes[present_mode_count];
    if (present_mode_count == 0)
    {
        printf("Failed to find available surface present modes\n");
    }
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, present_modes);

    for (uint32_t i = 0; i < format_count; i++)
    {
        details.formats[i] = formats[i];
    }

    for (uint32_t i = 0; i < present_mode_count; i++)
    {
        details.present_modes[i] = present_modes[i];
    }

    details.format_count = format_count;
    details.present_mode_count = present_mode_count;

    return details;
}

VkPhysicalDevice vk_gpu_init(VkInstance instance, VkSurfaceKHR surface)
{
    VkPhysicalDevice gpu = {0};

    uint32_t gpu_count = 0;
    vkEnumeratePhysicalDevices(instance, &gpu_count, NULL);

    if (gpu_count == 0)
    {
        printf("Failed to find GPUs with Vulkan support\n");
    }

    VkPhysicalDevice gpus[gpu_count];
    vkEnumeratePhysicalDevices(instance, &gpu_count, gpus);

    for (uint32_t i = 0; i < gpu_count; i++)
    {
        if (vk_gpu_suitability(gpus[i], surface))
        {
            gpu = gpus[i];
        }
    }

    if (gpu == VK_NULL_HANDLE)
    {
        printf("Failed to find an optimal GPU, going with first one found\n");
        gpu = gpus[0];
    }

    return gpu;
}
