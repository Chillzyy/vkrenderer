#include "gfx/vk/vk_cmd_buffers.h"

VkCommandBufferAllocateInfo vk_cmd_buffers_alloc_info_init(VkCommandPool cmd_pool, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    VkCommandBufferAllocateInfo alloc_info =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = cmd_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = MAX_FRAMES_IN_FLIGHT
    };

    return alloc_info;
}

VkCommandBufferBeginInfo vk_cmd_buffers_begin_info_init(VkCommandBufferUsageFlags flags)
{
    VkCommandBufferBeginInfo begin_info =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = flags,
        .pInheritanceInfo = NULL
    };

    return begin_info;
}

static VkRenderPassBeginInfo vk_cmd_buffers_render_pass_begin_info_init(vk_swapchain *s, uint32_t img_index, VkRenderPass render_pass)
{
    VkRenderPassBeginInfo render_pass_begin_info =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = NULL,
        .renderPass = render_pass,
        .framebuffer = s->framebuffers[img_index],
        .renderArea.offset = {0, 0},
        .renderArea.extent = s->img_extent,
        .clearValueCount = 1,
        .pClearValues = &s->clear_color
    };

    return render_pass_begin_info;
}

VkResult vk_cmd_buffers_init(vk_device *d, VkCommandBuffer *cmd_buffers, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    VkResult result = VK_SUCCESS;

    VkCommandBufferAllocateInfo alloc_info = vk_cmd_buffers_alloc_info_init(d->graphics_cmd_pool, MAX_FRAMES_IN_FLIGHT);

    result = vkAllocateCommandBuffers(d->device, &alloc_info, cmd_buffers);
    if (result != VK_SUCCESS)
    {
        printf("Failed to allocate command buffers\n");
        return result;
    }

    return result;
}

VkResult vk_cmd_buffers_record(vk_swapchain *s, VkPipeline pipeline, VkRenderPass render_pass, VkCommandBuffer cmd_buffers, VkBuffer vb, VkBuffer ib, uint16_t index_count, uint32_t img_index)
{
    VkResult result = VK_SUCCESS;

    VkCommandBufferBeginInfo cmd_buffers_begin_info = vk_cmd_buffers_begin_info_init(0);

    result = vkBeginCommandBuffer(cmd_buffers, &cmd_buffers_begin_info);
    if (result != VK_SUCCESS)
    {
        printf("Failed to begin recording command buffer\n");
    }

    VkRenderPassBeginInfo render_pass_begin_info = vk_cmd_buffers_render_pass_begin_info_init(s, img_index, render_pass);

    vkCmdBeginRenderPass(cmd_buffers, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd_buffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    vkCmdSetViewport(cmd_buffers, 0, 1, &s->viewport);
    vkCmdSetScissor(cmd_buffers, 0, 1, &s->scissor);

    VkDeviceSize offset[1] = {0};
    vkCmdBindVertexBuffers(cmd_buffers, 0, 1, &vb, offset);
    vkCmdBindIndexBuffer(cmd_buffers, ib, 0, VK_INDEX_TYPE_UINT16);

    vkCmdDrawIndexed(cmd_buffers, index_count, 1, 0, 0, 0);

    vkCmdEndRenderPass(cmd_buffers);

    result = vkEndCommandBuffer(cmd_buffers);
    if (result != VK_SUCCESS)
    {
        printf("Failed to record command buffer\n");
    }

    return result;
}
