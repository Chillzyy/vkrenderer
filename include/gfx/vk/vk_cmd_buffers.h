#ifndef vk_cmd_buffers_h
#define vk_cmd_buffers_h

#include "gfx/vk/vk_swapchain.h"

VkCommandBufferAllocateInfo vk_cmd_buffers_alloc_info_init(VkCommandPool cmd_pool, const uint32_t MAX_FRAMES_IN_FLIGHT);
VkCommandBufferBeginInfo vk_cmd_buffers_begin_info_init(VkCommandBufferUsageFlags flags);
VkResult vk_cmd_buffers_init(vk_device *d, VkCommandBuffer *cmd_buffers, const uint32_t MAX_FRAMES_IN_FLIGHT);
VkResult vk_cmd_buffers_record(vk_swapchain *s, VkPipeline pipeline, VkRenderPass render_pass, VkCommandBuffer cmd_buffers, VkBuffer vb, VkBuffer ib, uint16_t index_count, uint32_t img_index);

#endif
