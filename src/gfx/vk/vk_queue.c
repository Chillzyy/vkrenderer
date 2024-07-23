#include "gfx/vk/vk_queue.h"

static VkQueue vk_queue_init(VkDevice device, uint32_t queue_index)
{
    VkQueue queue = {0};
    vkGetDeviceQueue(device, queue_index, 0, &queue);
    return queue;
}

queue_family_indices vk_queue_families_init(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
    VkResult result = VK_SUCCESS;
    queue_family_indices indices = {0};

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, NULL);

    if (queue_family_count == 0)
    {
        printf("Failed to find queue families\n");
    }

    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queue_families);

    for (uint32_t i = 0; i < queue_family_count; i++)
    {
        VkBool32 graphics_support = queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
        VkBool32 present_support = false;
        VkBool32 transfer_support = queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT;
        result = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &present_support);


        if (graphics_support)
        {
            indices.graphics.index = i;
            indices.graphics.found = true;
        }

        if (result == VK_SUCCESS)
        {
            indices.present.index = i;
            indices.present.found = true;
        }

        if (transfer_support)
        {
            indices.transfer.index = i;
            indices.transfer.found = true;
        }
    }

    return indices;
}

vk_queues vk_queues_init(VkDevice device, queue_family_indices indices)
{
    vk_queues queues = {0};

    queues.graphics = vk_queue_init(device, indices.graphics.index);
    queues.present = vk_queue_init(device, indices.present.index);
    queues.transfer = vk_queue_init(device, indices.transfer.index);

    return queues;
}
