#ifndef vk_swapchain_h
#define vk_swapchain_h

#include "gfx/vk/vk_img_views.h"
#include "gfx/vk/vk_framebuffers.h"

typedef struct
{
    VkImage imgs[5];
    uint32_t img_count;
    uint32_t current_frame;

    VkFormat img_format;
    VkExtent2D img_extent;
    VkImageView img_views[5];

    VkViewport viewport;
    VkRect2D scissor;
    VkDynamicState dynamic_states[2];
    uint32_t DYNAMIC_STATE_COUNT;
    VkClearValue clear_color;

    VkFramebuffer framebuffers[5];

    VkSwapchainKHR swapchain;

}   vk_swapchain;

vk_swapchain vk_swapchain_init(vk_device *d);
vk_swapchain vk_swapchain_recreation(vk_device *d, VkRenderPass render_pass);
void vk_swapchain_destroy(vk_device *d, vk_swapchain *s);

#endif
