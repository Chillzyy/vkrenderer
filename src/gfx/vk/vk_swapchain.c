#include "gfx/vk/vk_swapchain.h"

static VkSurfaceFormatKHR vk_surface_format(const VkSurfaceFormatKHR *formats, uint32_t format_count)
{
    for (uint32_t i = 0; i < format_count; i++)
    {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return formats[i];
        }
    }

    return formats[0];
}

static VkPresentModeKHR vk_present_mode(const VkPresentModeKHR *present_modes, uint32_t present_mode_count)
{
    for (uint32_t i = 0; i < present_mode_count; i++)
    {
        if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return present_modes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D vk_swap_extent(GLFWwindow *window, const VkSurfaceCapabilitiesKHR extent)
{
    VkExtent2D extents = {0};

    if (extent.currentExtent.width == UINT32_MAX)
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        extents.width = width;
        extents.height = height;
    }
    else
    {
        extents = extent.currentExtent;
    }

    return extents;
}

static VkSwapchainCreateInfoKHR vk_swapchain_info_init(vk_device *d, swapchain_support_details swapchain_support, VkSurfaceFormatKHR surface_format, VkPresentModeKHR present_mode, VkExtent2D extent, VkSwapchainKHR old_swapchain, uint32_t image_count)
{
    VkSwapchainCreateInfoKHR swapchain_info =
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = d->surface,
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = swapchain_support.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = old_swapchain
    };

    uint32_t queue_family_indices[2] = {d->indices.graphics.index, d->indices.present.index};

    if (d->indices.graphics.index != d->indices.present.index)
    {
        swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_info.queueFamilyIndexCount = 2;
        swapchain_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_info.queueFamilyIndexCount = 0;
        swapchain_info.pQueueFamilyIndices = NULL;
    }

    return swapchain_info;
}

vk_swapchain vk_swapchain_init(vk_device *d)
{
    VkResult result = VK_SUCCESS;
    VkSwapchainKHR swapchain = {0};

    swapchain_support_details swapchain_support = vk_swapchain_support_init(d->gpu, d->surface);

    VkSurfaceFormatKHR surface_format = vk_surface_format(swapchain_support.formats, swapchain_support.format_count);
    VkPresentModeKHR present_mode = vk_present_mode(swapchain_support.present_modes, swapchain_support.present_mode_count);
    VkExtent2D extent = vk_swap_extent(d->window, swapchain_support.capabilities);

    uint32_t images_count = swapchain_support.capabilities.minImageCount + 1;

    if (swapchain_support.capabilities.maxImageCount > 0 && images_count > swapchain_support.capabilities.maxImageCount)
    {
        images_count = swapchain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_info = vk_swapchain_info_init(d, swapchain_support, surface_format, present_mode, extent, swapchain, images_count);

    result = vkCreateSwapchainKHR(d->device, &swapchain_info, NULL, &swapchain);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create swapchain\n");
    }

    vkGetSwapchainImagesKHR(d->device, swapchain, &images_count, NULL);
    VkImage images[images_count];
    vkGetSwapchainImagesKHR(d->device, swapchain, &images_count, images);

    VkImageView image_views[images_count];
    result = vk_img_views_init(d, images, images_count, surface_format.format, image_views);

    vk_swapchain s =
    {
        .img_count = images_count,
        .current_frame = 0,
        .img_format = surface_format.format,
        .img_extent = extent,
        .viewport = {.x = 0.0f, .y = 0.0f, .width = (float) s.img_extent.width, .height = (float) s.img_extent.height, .minDepth = 0.0f, .maxDepth = 1.0f},
        .scissor = {.offset = {0, 0}, .extent = s.img_extent},
        .dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT,
        .dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR,
        .DYNAMIC_STATE_COUNT = 2,
        .clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}},
        .swapchain = swapchain
    };

    for (uint32_t i = 0; i < images_count; i++)
    {
        s.imgs[i] = images[i];
        s.img_views[i] = image_views[i];
    }

    return s;
}

vk_swapchain vk_swapchain_recreation(vk_device *d, VkRenderPass render_pass)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(d->window, &width, &height);

    while (width == 0 || height == 0)
    {
        if (glfwWindowShouldClose(d->window))
        {
            break;
        }

        glfwGetFramebufferSize(d->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(d->device);

    VkFramebuffer framebuffers[5];
    vk_swapchain new = vk_swapchain_init(d);;

    vk_framebuffers_init(d, render_pass, new.img_extent, new.img_views, framebuffers, new.img_count);

    return new;
}

void vk_swapchain_destroy(vk_device *d, vk_swapchain *s)
{
    vk_framebuffers_destroy(d, s->framebuffers, s->img_count);
    vk_img_views_destroy(d, s->img_views, s->img_count);
    vkDestroySwapchainKHR(d->device, s->swapchain, NULL);
    s = NULL;
}
