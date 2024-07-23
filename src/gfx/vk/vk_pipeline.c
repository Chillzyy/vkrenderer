#include "gfx/vk/vk_pipeline.h"

static VkPipelineShaderStageCreateInfo vk_pipeline_shader_info_init(VkShaderModule module, VkShaderStageFlagBits stage, const char *entry_name)
{
    VkPipelineShaderStageCreateInfo shader_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stage = stage,
        .module = module,
        .pName = entry_name,
        .pSpecializationInfo = NULL
    };

    return shader_info;
}

static VkPipelineDynamicStateCreateInfo vk_pipeline_dynamic_state_info_init(vk_swapchain *s)
{
    VkPipelineDynamicStateCreateInfo dynamic_state_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .dynamicStateCount = s->DYNAMIC_STATE_COUNT,
        .pDynamicStates = s->dynamic_states
    };

    return dynamic_state_info;
}

static VkPipelineVertexInputStateCreateInfo vk_pipeline_vertex_input_info_init(VkVertexInputBindingDescription *binding_desc, uint32_t binding_desc_count, VkVertexInputAttributeDescription *attrib_descs, uint32_t attrib_desc_count)
{
    VkPipelineVertexInputStateCreateInfo vertex_input_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .vertexBindingDescriptionCount = binding_desc_count,
        .pVertexBindingDescriptions = binding_desc,
        .vertexAttributeDescriptionCount = attrib_desc_count,
        .pVertexAttributeDescriptions = attrib_descs
    };

    return vertex_input_info;
}

static VkPipelineInputAssemblyStateCreateInfo vk_pipeline_input_assembly_info_init(void)
{
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    return input_assembly_info;
}

static VkPipelineViewportStateCreateInfo vk_pipeline_viewport_state_info_init(void)
{
    VkPipelineViewportStateCreateInfo viewport_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = NULL,
        .scissorCount = 1,
        .pScissors = NULL
    };

    return viewport_info;
}

static VkPipelineRasterizationStateCreateInfo vk_pipeline_rasterization_info_init(void)
{
    VkPipelineRasterizationStateCreateInfo rasterization_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f
    };

    return rasterization_info;
}

static VkPipelineMultisampleStateCreateInfo vk_pipeline_multisample_info_init(void)
{
    VkPipelineMultisampleStateCreateInfo multisample_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading = 1.0f,
        .pSampleMask = NULL,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };

    return multisample_info;
}

static VkPipelineColorBlendAttachmentState vk_pipeline_color_blend_attachment_info_init(void)
{
    VkPipelineColorBlendAttachmentState color_blend_attachment_info =
    {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    return color_blend_attachment_info;
}

static VkPipelineColorBlendStateCreateInfo vk_pipeline_color_blend_info_init(VkPipelineColorBlendAttachmentState *color_blend_attachment)
{
    VkPipelineColorBlendStateCreateInfo color_blend_info =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = color_blend_attachment,
        .blendConstants[0] = 0.0f,
        .blendConstants[1] = 0.0f,
        .blendConstants[2] = 0.0f,
        .blendConstants[3] = 0.0f
    };

    return color_blend_info;
}

static VkGraphicsPipelineCreateInfo vk_pipeline_info_init(VkPipelineShaderStageCreateInfo *shader_stages, uint32_t shader_stage_count, VkPipelineVertexInputStateCreateInfo *vertex_input, VkPipelineInputAssemblyStateCreateInfo *input_assembly, VkPipelineViewportStateCreateInfo *viewport, VkPipelineRasterizationStateCreateInfo *rasterization, VkPipelineMultisampleStateCreateInfo *multisample, VkPipelineColorBlendStateCreateInfo *color_blend, VkPipelineDynamicStateCreateInfo *dynamic_state, VkPipelineLayout pipeline_layout, VkRenderPass render_pass)
{
    VkGraphicsPipelineCreateInfo pipeline_info =
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stageCount = shader_stage_count,
        .pStages = shader_stages,
        .pVertexInputState = vertex_input,
        .pInputAssemblyState = input_assembly,
        .pTessellationState = NULL,
        .pViewportState = viewport,
        .pRasterizationState = rasterization,
        .pMultisampleState = multisample,
        .pDepthStencilState = NULL,
        .pColorBlendState = color_blend,
        .pDynamicState = dynamic_state,
        .layout = pipeline_layout,
        .renderPass = render_pass,
        .subpass = 0,
        .basePipelineHandle = NULL,
        .basePipelineIndex = -1
    };

    return pipeline_info;
}

vk_pipeline vk_pipeline_init(vk_device *d, vk_swapchain *s, const uint32_t MAX_FRAMES_IN_FLIGHT)
{
    VkResult result = VK_SUCCESS;
    VkPipeline pipeline = {0};

    vk_shaders shaders = vk_shaders_init(d);

    VkPipelineShaderStageCreateInfo vertex_stage = vk_pipeline_shader_info_init(shaders.vertex_module, VK_SHADER_STAGE_VERTEX_BIT, shaders.entry_name);
    VkPipelineShaderStageCreateInfo fragment_stage = vk_pipeline_shader_info_init(shaders.fragment_module, VK_SHADER_STAGE_FRAGMENT_BIT, shaders.entry_name);

    uint32_t shader_stage_count = 2;
    VkPipelineShaderStageCreateInfo shader_stages[2] = {vertex_stage, fragment_stage};

    uint32_t binding_desc_count = 1;
    uint32_t attrib_desc_count = 2;

    VkVertexInputBindingDescription binding_desc = vk_vertex_binding_desc();

    VkVertexInputAttributeDescription pos = vk_vertex_attrib_pos();
    VkVertexInputAttributeDescription color = vk_vertex_attrib_color();

    VkVertexInputAttributeDescription attrib_descs[2] = {pos, color};

    VkPipelineVertexInputStateCreateInfo vertex_input = vk_pipeline_vertex_input_info_init(&binding_desc, binding_desc_count, attrib_descs, attrib_desc_count);
    VkPipelineInputAssemblyStateCreateInfo input_assembly = vk_pipeline_input_assembly_info_init();
    VkPipelineViewportStateCreateInfo viewport = vk_pipeline_viewport_state_info_init();
    VkPipelineRasterizationStateCreateInfo rasterization = vk_pipeline_rasterization_info_init();
    VkPipelineMultisampleStateCreateInfo multisample = vk_pipeline_multisample_info_init();
    VkPipelineColorBlendAttachmentState attachment = vk_pipeline_color_blend_attachment_info_init();
    VkPipelineColorBlendStateCreateInfo color_blend = vk_pipeline_color_blend_info_init(&attachment);
    VkPipelineDynamicStateCreateInfo dynamic_state = vk_pipeline_dynamic_state_info_init(s);

    VkPipelineLayout pipeline_layout = vk_pipeline_layout_init(d);
    VkRenderPass render_pass = vk_render_pass_init(d, s);

    VkCommandBuffer cmd_buffers[2];
    result = vk_cmd_buffers_init(d, cmd_buffers, MAX_FRAMES_IN_FLIGHT);

    VkGraphicsPipelineCreateInfo pipeline_info = vk_pipeline_info_init(shader_stages, shader_stage_count, &vertex_input, &input_assembly, &viewport, &rasterization, &multisample, &color_blend, &dynamic_state, pipeline_layout, render_pass);

    result = vkCreateGraphicsPipelines(d->device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create graphics pipeline\n");
    }

    vk_pipeline p =
    {
        .render_pass = render_pass,
        .pipeline_layout = pipeline_layout,
        .pipeline = pipeline
    };

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        p.cmd_buffers[i] = cmd_buffers[i];
    }

    vk_shaders_destroy(d, &shaders);

    return p;
}

void vk_pipeline_destroy(vk_device *d, vk_pipeline *p)
{
    vk_render_pass_destroy(d, p->render_pass);
    vk_pipeline_layout_destroy(d, p->pipeline_layout);
    vkDestroyPipeline(d->device, p->pipeline, NULL);
    p = NULL;
}
