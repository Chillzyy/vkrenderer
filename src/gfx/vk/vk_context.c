#include "gfx/vk/vk_context.h"

vk_vertex vertices[4] =
{
    {{ -0.5f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{  0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{  0.5f,   0.5f}, {0.0f, 0.0f, 1.0f}},
    {{ -0.5f,   0.5f}, {1.0f, 1.0f, 1.0f}}
};

uint32_t vertex_count = 4;

uint16_t indices[6] = {0, 1, 2, 2, 3, 0};
uint16_t index_count = 6;

size_t vb_size = sizeof(vertices) / sizeof(vertices[0]) * sizeof(vk_vertex);
size_t ib_size = sizeof(indices) / sizeof(indices[0]) * sizeof(uint16_t);

vk_context vk_context_init(const char *title, bool debugging)
{
    vk_context vk = {0};

    vk.MAX_FRAMES_IN_FLIGHT = 2;
    vk.d = vk_device_init(title, debugging);
    vk.s = vk_swapchain_init(&vk.d);
    vk.p = vk_pipeline_init(&vk.d, &vk.s, vk.MAX_FRAMES_IN_FLIGHT);

    vk_framebuffers_init(&vk.d, vk.p.render_pass, vk.s.img_extent, vk.s.img_views, vk.s.framebuffers, vk.s.img_count);

    vk.vb = vk_vertex_buffer_init(&vk.d, vertices, vb_size);
    vk.ib = vk_index_buffer_init(&vk.d, indices, ib_size);

    vk_sync_obj_init(&vk.d, &vk.so, vk.MAX_FRAMES_IN_FLIGHT);

    return vk;
}

void vk_context_mainloop(vk_context *vk)
{
    while (!glfwWindowShouldClose(vk->d.window))
    {
        glfwPollEvents();
        vk_present_img(&vk->d, &vk->s, &vk->p, &vk->vb, &vk->ib, &vk->so, index_count, vk->MAX_FRAMES_IN_FLIGHT);
    }

    vkDeviceWaitIdle(vk->d.device);
}

void vk_context_destroy(vk_context *vk, bool debugging)
{
    vk_swapchain_destroy(&vk->d, &vk->s);
    vk_pipeline_destroy(&vk->d, &vk->p);
    vk_buffers_destroy(&vk->d, &vk->vb, &vk->ib);
    vk_sync_obj_destroy(&vk->d, &vk->so, vk->MAX_FRAMES_IN_FLIGHT);
    vk_device_destroy(&vk->d, debugging);
}
