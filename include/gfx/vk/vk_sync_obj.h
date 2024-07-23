#ifndef vk_sync_obj_h
#define vk_sync_obj_h

#include "gfx/vk/vk_device.h"

typedef struct
{
    VkSemaphore img_available[2];
    VkSemaphore render_finished[2];
    VkFence in_flight[2];
}   vk_sync_obj;

VkResult vk_sync_obj_init(vk_device *d, vk_sync_obj *so, const uint32_t MAX_FRAMES_IN_FLIGHT);
void vk_sync_obj_destroy(vk_device *d, vk_sync_obj *so, const uint32_t MAX_FRAMES_IN_FLIGHT);

#endif
