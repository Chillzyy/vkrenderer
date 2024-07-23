#include "gfx/vk/vk_sync_obj.h"

static VkSemaphoreCreateInfo vk_semaphore_info_init(void)
{
    VkSemaphoreCreateInfo semaphore_info =
    {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0
    };

    return semaphore_info;
}

static VkFenceCreateInfo vk_fence_info_init(void)
{
    VkFenceCreateInfo fence_info =
    {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    return fence_info;
}

VkResult vk_sync_obj_init(vk_device *d, vk_sync_obj *so, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    VkResult result = VK_SUCCESS;

    VkSemaphoreCreateInfo semaphore_info = vk_semaphore_info_init();
    VkFenceCreateInfo fence_info = vk_fence_info_init();

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        // Flagged when program waits for command buffers
        result = vkCreateSemaphore(d->device, &semaphore_info, NULL, &so->img_available[i]);
        if (result != VK_SUCCESS)
        {
            printf("Failed to create image available semaphore\n");
            return result;
        }

        // Flagged when program is done using selected command buffers
        result = vkCreateSemaphore(d->device, &semaphore_info, NULL, &so->render_finished[i]);
        if (result != VK_SUCCESS)
        {
            printf("Failed to create render finished semaphore\n");
            return result;
        }

        // Flagged when previously presented frame has finished presenting
        result = vkCreateFence(d->device, &fence_info, NULL, &so->in_flight[i]);
        if (result != VK_SUCCESS)
        {
            printf("Failed to create in flight fence\n");
            return result;
        }
    }

    return result;
}

void vk_sync_obj_destroy(vk_device *d, vk_sync_obj *so, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(d->device, so->img_available[i], NULL);
        vkDestroySemaphore(d->device, so->render_finished[i], NULL);
        vkDestroyFence(d->device, so->in_flight[i], NULL);
    }

    so = NULL;
}
