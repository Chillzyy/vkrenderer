#ifndef vk_present_img_h
#define vk_present_img_h

#include "gfx/vk/vk_pipeline.h"
#include "gfx/vk/vk_buffer.h"
#include "gfx/vk/vk_sync_obj.h"

void vk_present_img(vk_device *d, vk_swapchain *s, vk_pipeline *p, vk_buffer *vb, vk_buffer *ib, vk_sync_obj *so, uint16_t index_count, const uint32_t MAX_FRAMES_IN_FLIGHT);

#endif
