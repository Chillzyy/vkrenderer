#ifndef vk_pipeline_h
#define vk_pipeline_h

#include "gfx/vk/vk_vertex.h"
#include "gfx/vk/vk_shaders.h"
#include "gfx/vk/vk_cmd_buffers.h"
#include "gfx/vk/vk_pipeline_layout.h"
#include "gfx/vk/vk_render_pass.h"

typedef struct
{
    VkCommandBuffer cmd_buffers[2];
    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;
}   vk_pipeline;

vk_pipeline vk_pipeline_init(vk_device *d, vk_swapchain *s, const uint32_t MAX_FRAMES_IN_FLIGHT);
void vk_pipeline_destroy(vk_device *d, vk_pipeline *p);

#endif
