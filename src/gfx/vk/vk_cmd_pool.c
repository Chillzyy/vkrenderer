#include "gfx/vk/vk_cmd_pool.h"

static VkCommandPoolCreateInfo vk_cmd_pool_info_init(VkCommandPoolCreateFlags flags, uint32_t queue_family_index)
{
    VkCommandPoolCreateInfo cmd_pool_info =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = flags,
        .queueFamilyIndex = queue_family_index
    };

    return cmd_pool_info;
}

VkCommandPool vk_cmd_pool_init(VkDevice device, VkCommandPoolCreateFlags flags, uint32_t queue_family_index)
{
    VkResult result = VK_SUCCESS;
    VkCommandPool cmd_pool = {0};

    VkCommandPoolCreateInfo cmd_pool_info = vk_cmd_pool_info_init(flags, queue_family_index);

    result = vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmd_pool);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create command pool\n");
    }

    return cmd_pool;
}

void vk_cmd_pool_destroy(VkDevice device, VkCommandPool cmd_pool)
{
    vkDestroyCommandPool(device, cmd_pool, NULL);
}
