#ifndef vk_queue_h
#define vk_queue_h

#include <stdio.h>
#include <stdbool.h>
#include "vulkan/vulkan.h"

typedef struct
{
    uint32_t index;
    bool found;
}   queue_family;

typedef struct
{
    queue_family graphics;
    queue_family present;
    queue_family transfer;
}   queue_family_indices;

typedef struct
{
    VkQueue graphics;
    VkQueue present;
    VkQueue transfer;
}   vk_queues;

queue_family_indices vk_queue_families_init(VkPhysicalDevice gpu, VkSurfaceKHR surface);
vk_queues vk_queues_init(VkDevice device, queue_family_indices indices);

#endif
