#include "gfx/vk/vk_img_views.h"

static VkImageViewCreateInfo vk_img_view_info_init(VkImage img, VkFormat img_format)
{
    VkImageViewCreateInfo img_view_info =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .image = img,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = img_format,
        .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel = 0,
        .subresourceRange.levelCount = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount = 1
    };

    return img_view_info;
}

// This is returning VkResult because you can't return a local array (pointer) from a function
VkResult vk_img_views_init(vk_device *d, VkImage *imgs, uint32_t img_count, VkFormat img_format, VkImageView *img_views)
{
    VkResult result = VK_SUCCESS;

    for (uint32_t i = 0; i < img_count; i++)
    {
        VkImageViewCreateInfo img_view_info = vk_img_view_info_init(imgs[i], img_format);
        result = vkCreateImageView(d->device, &img_view_info, NULL, &img_views[i]);
        if (result != VK_SUCCESS)
        {
            printf("Failed to create image views\n");
            return result;
        }
    }

    return result;
}

void vk_img_views_destroy(vk_device *d, VkImageView *img_views, uint32_t img_count)
{
    for (uint32_t i = 0; i < img_count; i++)
    {
        vkDestroyImageView(d->device, img_views[i], NULL);
    }
}
