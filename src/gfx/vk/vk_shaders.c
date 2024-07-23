#include "gfx/vk/vk_shaders.h"

// All this just to avoid the heap... ¯\_(ツ)_/¯
// this is disgusting
static void vk_shader_size_get(const char *filename, size_t *length)
{
    FILE* fptr = fopen(filename, "rb");

    if (fptr == NULL)
    {
        printf("Failed to get file size\n");
    }

    fseek(fptr, 0L, SEEK_END);
    long len = ftell(fptr);
    *length = (size_t) len;

    if (len < 0)
    {
        printf("Failed to determine the size of the file\n");
    }

    else if (*length != (size_t) len)
    {
        printf("A conversion overflow has occurred\n");
        fseek(fptr, 0L, SEEK_SET);
        fclose(fptr);
        return;
    }

    fseek(fptr, 0L, SEEK_SET);
    fclose(fptr);
}

static uint32_t *vk_shaders_load(const char *filename, size_t length, char *contents)
{
    FILE* fptr = fopen(filename, "rb");

    if (fptr == NULL)
    {
        printf("Failed to open file\n");
    }

    fseek(fptr, 0L, SEEK_END);
    length = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    size_t read_count = fread(contents, length, 1, fptr);

    if (read_count == 0)
    {
        printf("Failed to read SPIR-V files\n");
    }

    fclose(fptr);
    return (uint32_t *) contents;
}

static VkShaderModuleCreateInfo vk_shader_module_info_init(const uint32_t *shader, const size_t shader_size)
{
    VkShaderModuleCreateInfo module_info =
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .codeSize = shader_size,
        .pCode = shader
    };

    return module_info;
}

static VkShaderModule vk_shader_module_init(vk_device *d, const uint32_t *shader, const size_t shader_size)
{
    VkResult result = VK_SUCCESS;
    VkShaderModule shader_module = {0};

    VkShaderModuleCreateInfo module_info = vk_shader_module_info_init(shader, shader_size);

    result = vkCreateShaderModule(d->device, &module_info, NULL, &shader_module);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create shader module\n");
    }

    return shader_module;
}

static VkShaderModule vk_vertex_shader_init(vk_device *d, const char *path)
{
    size_t vert_spirv_size = 0;
    vk_shader_size_get(path, &vert_spirv_size);
    char vert_contents[vert_spirv_size + 1];
    uint32_t *vert_spirv = vk_shaders_load(path, vert_spirv_size, vert_contents);

    if (vert_spirv == NULL)
    {
        printf("Failed to load vertex shader\n");
    }

    VkShaderModule vertex = vk_shader_module_init(d, vert_spirv, vert_spirv_size);

    return vertex;
}

static VkShaderModule vk_fragment_shader_init(vk_device *d, const char *path)
{
    size_t frag_spirv_size = 0;
    vk_shader_size_get(path, &frag_spirv_size);
    char frag_contents[frag_spirv_size + 1];
    uint32_t *frag_spirv = vk_shaders_load(path, frag_spirv_size, frag_contents);

    if (frag_spirv == NULL)
    {
        printf("Failed to load fragment shader\n");
    }

    VkShaderModule fragment = vk_shader_module_init(d, frag_spirv, frag_spirv_size);

    return fragment;
}

vk_shaders vk_shaders_init(vk_device *d)
{
    vk_shaders s =
    {
        .vertex_shader_path = "renderer/shaders/vertex.vert.spv",
        .fragment_shader_path = "renderer/shaders/fragment.frag.spv",
        .entry_name = "main",
        .vertex_module = vk_vertex_shader_init(d, s.vertex_shader_path),
        .fragment_module = vk_fragment_shader_init(d, s.fragment_shader_path)
    };

    return s;
}

void vk_shaders_destroy(vk_device *d, vk_shaders *s)
{
    vkDestroyShaderModule(d->device, s->vertex_module, NULL);
    vkDestroyShaderModule(d->device, s->fragment_module, NULL);
    s = NULL;
}
