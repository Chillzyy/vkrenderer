#ifndef vk_context_h
#define vk_context_h

#include "gfx/vk/vk_present_img.h"

typedef struct
{
    uint32_t MAX_FRAMES_IN_FLIGHT;

    vk_device d;
    vk_swapchain s;
    vk_pipeline p;
    vk_sync_obj so;
    vk_buffer vb;
    vk_buffer ib;
}   vk_context;

vk_context vk_context_init(const char *title, bool debugging);
void vk_context_mainloop(vk_context *vk);
void vk_context_destroy(vk_context *vk, bool debugging);

#endif
