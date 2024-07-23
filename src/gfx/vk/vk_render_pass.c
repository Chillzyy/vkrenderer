#include "gfx/vk/vk_render_pass.h"

static VkAttachmentDescription vk_attachment_description_info_init(vk_swapchain *s)
{
    VkAttachmentDescription attachment_description =
    {
        .flags = 0,
        .format = s->img_format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    return attachment_description;
}

static VkAttachmentReference vk_attachment_reference_info_init(void)
{
    VkAttachmentReference attachment_reference =
    {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    return attachment_reference;
}

static VkSubpassDescription vk_subpass_info_init(VkAttachmentReference *reference)
{
    VkSubpassDescription subpass_info =
    {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = NULL,
        .colorAttachmentCount = 1,
        .pColorAttachments = reference,
        .pResolveAttachments = NULL,
        .pDepthStencilAttachment = NULL,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = NULL
    };

    return subpass_info;
}

static VkSubpassDependency vk_subpass_dependency_info_init(void)
{
    VkSubpassDependency subpass_dependency =
    {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = 0
    };

    return subpass_dependency;
}

static VkRenderPassCreateInfo vk_render_pass_info_init(VkAttachmentDescription *attachment, VkSubpassDescription *subpass, VkSubpassDependency *dependency)
{
    VkRenderPassCreateInfo render_pass_info =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .attachmentCount = 1,
        .pAttachments = attachment,
        .subpassCount = 1,
        .pSubpasses = subpass,
        .dependencyCount = 1,
        .pDependencies = dependency
    };

    return render_pass_info;
}

VkRenderPass vk_render_pass_init(vk_device *d, vk_swapchain *s)
{
    VkResult result = VK_SUCCESS;
    VkRenderPass render_pass = {0};

    VkAttachmentDescription color_attachment = vk_attachment_description_info_init(s);
    VkAttachmentReference color_attachment_ref = vk_attachment_reference_info_init();
    VkSubpassDescription subpass = vk_subpass_info_init(&color_attachment_ref);
    VkSubpassDependency dependency = vk_subpass_dependency_info_init();

    VkRenderPassCreateInfo render_pass_info = vk_render_pass_info_init(&color_attachment, &subpass, &dependency);

    result = vkCreateRenderPass(d->device, &render_pass_info, NULL, &render_pass);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create render pass\n");
    }

    return render_pass;
}

void vk_render_pass_destroy(vk_device *d, VkRenderPass render_pass)
{
    vkDestroyRenderPass(d->device, render_pass, NULL);
}
