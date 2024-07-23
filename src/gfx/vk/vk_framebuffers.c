#include "gfx/vk/vk_framebuffers.h"

static VkFramebufferCreateInfo vk_framebuffer_info_init(VkRenderPass render_pass, VkImageView *img_views, VkExtent2D img_extent)
{
    VkFramebufferCreateInfo framebuffer_info =
    {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .renderPass = render_pass,
        .attachmentCount = 1,
        .pAttachments = img_views,
        .width = img_extent.width,
        .height = img_extent.height,
        .layers = 1
    };

    return framebuffer_info;
}

VkResult vk_framebuffers_init(vk_device *d, VkRenderPass render_pass, VkExtent2D img_extent, VkImageView *img_views, VkFramebuffer *framebuffers, uint32_t img_count)
{
    VkResult result = VK_SUCCESS;

    for (uint32_t i = 0; i < img_count; i++)
    {
        VkFramebufferCreateInfo framebuffer_info = vk_framebuffer_info_init(render_pass, &img_views[i], img_extent);
        result = vkCreateFramebuffer(d->device, &framebuffer_info, NULL, &framebuffers[i]);
        if (result != VK_SUCCESS)
        {
            printf("Failed to create framebuffers\n");
            return result;
        }
    }

    return result;
}

void vk_framebuffers_destroy(vk_device *d, VkFramebuffer *framebuffers, uint32_t img_count)
{
    for (uint32_t i = 0; i < img_count; i++)
    {
        vkDestroyFramebuffer(d->device, framebuffers[i], NULL);
    }
}
