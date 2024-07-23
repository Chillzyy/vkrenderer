#include "gfx/vk/vk_pipeline_layout.h"

static VkPipelineLayoutCreateInfo vk_pipeline_layout_info_init(void)
{
    VkPipelineLayoutCreateInfo pipeline_layout_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = NULL,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL
    };

    return pipeline_layout_info;
}

VkPipelineLayout vk_pipeline_layout_init(vk_device *d)
{
    VkResult result = VK_SUCCESS;
    VkPipelineLayout pipeline_layout = {0};

    VkPipelineLayoutCreateInfo pipeline_layout_info = vk_pipeline_layout_info_init();

    result = vkCreatePipelineLayout(d->device, &pipeline_layout_info, NULL, &pipeline_layout);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create pipeline layout\n");
    }

    return pipeline_layout;
}

void vk_pipeline_layout_destroy(vk_device *d, VkPipelineLayout pipeline_layout)
{
    vkDestroyPipelineLayout(d->device, pipeline_layout, NULL);
}
