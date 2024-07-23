#include "gfx/vk/vk_present_img.h"

static VkSubmitInfo vk_submit_info_init(vk_swapchain *s, vk_pipeline *p, VkPipelineStageFlags stages, vk_sync_obj *so)
{
    VkSubmitInfo submit_info =
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &so->img_available[s->current_frame],
        .pWaitDstStageMask = &stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &p->cmd_buffers[s->current_frame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &so->render_finished[s->current_frame]
    };

    return submit_info;
}

static VkPresentInfoKHR vk_present_info_init(vk_swapchain *s, vk_sync_obj *so, uint32_t *img_index)
{
    VkPresentInfoKHR present_info =
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = NULL,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &so->render_finished[s->current_frame],
        .swapchainCount = 1,
        .pSwapchains = &s->swapchain,
        .pImageIndices = img_index,
        .pResults = NULL
    };

    return present_info;
}

void vk_present_img(vk_device *d, vk_swapchain *s, vk_pipeline *p, vk_buffer *vb, vk_buffer *ib, vk_sync_obj *so, uint16_t index_count, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    VkResult result = VK_SUCCESS;

    result = vkWaitForFences(d->device, 1, &so->in_flight[s->current_frame], VK_TRUE, UINT64_MAX);
    if (result != VK_SUCCESS)
    {
        printf("Failed to wait for fences\n");
    }

    uint32_t img_index = 0;
    result = vkAcquireNextImageKHR(d->device, s->swapchain, UINT64_MAX, so->img_available[s->current_frame], VK_NULL_HANDLE, &img_index);

    switch (result)
    {
        case VK_SUBOPTIMAL_KHR:
        case VK_ERROR_OUT_OF_DATE_KHR:
            *s = vk_swapchain_recreation(d, p->render_pass);
            break;

        case VK_SUCCESS:
            break;

        default:
            printf("Failed to acquire next swapchain image\n");
            break;
    }

    result = vkResetFences(d->device, 1, &so->in_flight[s->current_frame]);
    if (result != VK_SUCCESS)
    {
        printf("Failed to reset fences\n");
    }

    result = vkResetCommandBuffer(p->cmd_buffers[s->current_frame], 0);
    if (result != VK_SUCCESS)
    {
        printf("Failed to reset command buffer\n");
    }

    vk_cmd_buffers_record(s, p->pipeline, p->render_pass, p->cmd_buffers[s->current_frame], vb->buffer, ib->buffer, index_count, img_index);

    VkSubmitInfo submit_info = vk_submit_info_init(s, p, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, so);

    result = vkQueueSubmit(d->queues.graphics, 1, &submit_info, so->in_flight[s->current_frame]);
    if (result != VK_SUCCESS)
    {
        printf("Failed to submit draw command buffer\n");
    }

    VkPresentInfoKHR present_info = vk_present_info_init(s, so, &img_index);

    result = vkQueuePresentKHR(d->queues.present, &present_info);

    switch (result)
    {
        case VK_SUBOPTIMAL_KHR:
        case VK_ERROR_OUT_OF_DATE_KHR:
            *s = vk_swapchain_recreation(d, p->render_pass);
            if (framebuffer_resized)
                framebuffer_resized = false;
            break;

        case VK_SUCCESS:
            break;

        default:
            printf("Failed to present image\n");
            break;
    }

    //https://vulkan-tutorial.com/en/Drawing_a_triangle/Drawing/Frames_in_flight#comment-6378283909
    s->current_frame = s->current_frame + 1;
    s->current_frame *= s->current_frame < MAX_FRAMES_IN_FLIGHT;

}
