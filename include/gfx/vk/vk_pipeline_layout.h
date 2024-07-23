#ifndef vk_pipeline_layout_h
#define vk_pipeline_layout_h

#include "gfx/vk/vk_device.h"

VkPipelineLayout vk_pipeline_layout_init(vk_device *d);
void vk_pipeline_layout_destroy(vk_device *d, VkPipelineLayout pipeline_layout);

#endif
