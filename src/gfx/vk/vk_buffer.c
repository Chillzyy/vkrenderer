#include "gfx/vk/vk_buffer.h"

static uint32_t vk_find_memory_type(vk_device *d, uint32_t type_filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties mem_properties = {0};
    vkGetPhysicalDeviceMemoryProperties(d->gpu, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
    {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    printf("Failed to find a suitable memory type\n");
    return 0;
}

static VkMemoryAllocateInfo vk_buffer_memory_alloc_info_init(vk_device *d, VkMemoryRequirements mem_requirements, VkMemoryPropertyFlags properties)
{
    VkMemoryAllocateInfo alloc_info =
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,
        .allocationSize = mem_requirements.size,
        .memoryTypeIndex = vk_find_memory_type(d, mem_requirements.memoryTypeBits, properties)
    };

    return alloc_info;
}

static VkBufferCreateInfo vk_buffer_info_init(bool separate_queues, uint32_t *queue_familiies, uint32_t queue_family_count, VkBufferUsageFlags usage, size_t size)
{
    VkBufferCreateInfo buffer_info =
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .size = size,
        .usage = usage
    };

    if (separate_queues)
    {
        buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount = queue_family_count;
        buffer_info.pQueueFamilyIndices = queue_familiies;
    }
    else
    {
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        buffer_info.queueFamilyIndexCount = 0;
        buffer_info.pQueueFamilyIndices = NULL;
    }

    return buffer_info;
}

static VkSubmitInfo vk_buffer_submit_info(VkCommandBuffer *cmd_buffer, uint32_t cmd_buffer_count)
{
    VkSubmitInfo submit_info =
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = NULL,
        .pWaitDstStageMask = NULL,
        .commandBufferCount = cmd_buffer_count,
        .pCommandBuffers = cmd_buffer,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = NULL
    };

    return submit_info;
}

static VkResult vk_buffer_copy(vk_device *d, vk_buffer src, vk_buffer dst, VkDeviceSize size)
{
    VkResult result = VK_SUCCESS;

    const uint32_t cmd_buffer_count = 1;
    VkCommandBufferAllocateInfo alloc_info = vk_cmd_buffers_alloc_info_init(d->transfer_cmd_pool, cmd_buffer_count);
    VkCommandBuffer cmd_buffer = {0};

    result = vkAllocateCommandBuffers(d->device, &alloc_info, &cmd_buffer);
    if (result != VK_SUCCESS)
    {
        printf("Failed to allocate command buffer\n");
        return result;
    }

    VkCommandBufferBeginInfo begin_info = vk_cmd_buffers_begin_info_init(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    result = vkBeginCommandBuffer(cmd_buffer, &begin_info);
    if (result != VK_SUCCESS)
    {
        printf("Failed to begin command buffer\n");
        return result;
    }

    VkBufferCopy copy_region =
    {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };

    vkCmdCopyBuffer(cmd_buffer, src.buffer, dst.buffer, 1, &copy_region);

    result = vkEndCommandBuffer(cmd_buffer);
    if (result != VK_SUCCESS)
    {
        printf("Failed to end command buffer\n");
        return result;
    }

    VkSubmitInfo submit_info = vk_buffer_submit_info(&cmd_buffer, 1);

    result = vkQueueSubmit(d->queues.transfer, 1, &submit_info, VK_NULL_HANDLE);
    if (result != VK_SUCCESS)
    {
        printf("Failed to submit queue\n");
        return result;
    }

    result = vkQueueWaitIdle(d->queues.transfer);
    if (result != VK_SUCCESS)
    {
        printf("Failed to wait for transfer queue\n");
        return result;
    }

    vkFreeCommandBuffers(d->device, d->transfer_cmd_pool, 1, &cmd_buffer);

    return result;
}

static VkBuffer vk_buffer_create(vk_device *d, VkBufferUsageFlags usage, size_t size)
{
    VkResult result = VK_SUCCESS;
    VkBuffer buffer = {0};

    uint32_t queue_families[2] = {d->indices.graphics.index, d->indices.transfer.index};
    uint32_t queue_family_count = 2;
    bool separate_queues = (d->indices.graphics.index != d->indices.transfer.index) ? 1 : 0;

    VkBufferCreateInfo buffer_info = vk_buffer_info_init(separate_queues, queue_families, queue_family_count, usage, size);

    result = vkCreateBuffer(d->device, &buffer_info, NULL, &buffer);
    if (result != VK_SUCCESS)
    {
        printf("Failed to cerate vertex buffer\n");
    }

    return buffer;
}

static VkDeviceMemory vk_buffer_memory_init(vk_device *d, VkBuffer buffer, VkMemoryPropertyFlags properties)
{
    VkResult result = VK_SUCCESS;
    VkDeviceMemory memory = {0};

    VkMemoryRequirements mem_requirements = {0};
    vkGetBufferMemoryRequirements(d->device, buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info = vk_buffer_memory_alloc_info_init(d, mem_requirements, properties);

    result = vkAllocateMemory(d->device, &alloc_info, NULL, &memory);
    if (result != VK_SUCCESS)
    {
        printf("Failed to allocate vertex buffer memory\n");
    }

    result = vkBindBufferMemory(d->device, buffer, memory, 0);
    if (result != VK_SUCCESS)
    {
        printf("Failed to bind buffer memory\n");
    }

    return memory;
}

static vk_buffer vk_buffer_init(vk_device *d, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, size_t size)
{
    VkBuffer buffer = vk_buffer_create(d, usage, size);
    VkDeviceMemory memory = vk_buffer_memory_init(d, buffer, properties);

    vk_buffer b =
    {
        .memory = memory,
        .buffer = buffer
    };

    return b;
}

vk_buffer vk_vertex_buffer_init(vk_device *d, vk_vertex *vertices, size_t vb_size)
{
    VkResult result = VK_SUCCESS;

    vk_buffer sb = vk_buffer_init(d, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vb_size);

    void *data = 0;
    vkMapMemory(d->device, sb.memory, 0, vb_size, 0, &data);
    memcpy(data, vertices, vb_size);
    vkUnmapMemory(d->device, sb.memory);

    vk_buffer vb = vk_buffer_init(d, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vb_size);

    result = vk_buffer_copy(d, sb, vb, vb_size);
    if (result != VK_SUCCESS)
    {
        printf("Failed to copy buffer\n");
    }

    vkDestroyBuffer(d->device, sb.buffer, NULL);
    vkFreeMemory(d->device, sb.memory, NULL);

    return vb;
}

vk_buffer vk_index_buffer_init(vk_device *d, uint16_t *indices, size_t ib_size)
{
    VkResult result = VK_SUCCESS;

    vk_buffer sb = vk_buffer_init(d, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, ib_size);

    void *data = 0;
    vkMapMemory(d->device, sb.memory, 0, ib_size, 0, &data);
    memcpy(data, indices, ib_size);
    vkUnmapMemory(d->device, sb.memory);

    vk_buffer ib = vk_buffer_init(d, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ib_size);

    result = vk_buffer_copy(d, sb, ib, ib_size);
    if (result != VK_SUCCESS)
    {
        printf("Failed to copy buffer\n");
    }

    vkDestroyBuffer(d->device, sb.buffer, NULL);
    vkFreeMemory(d->device, sb.memory, NULL);

    return ib;
}

void vk_buffers_destroy(vk_device *d, vk_buffer *vb, vk_buffer *ib)
{
    vkDestroyBuffer(d->device, vb->buffer, NULL);
    vkFreeMemory(d->device, vb->memory, NULL);
    vkDestroyBuffer(d->device, ib->buffer, NULL);
    vkFreeMemory(d->device, ib->memory, NULL);
    vb = NULL;
    ib = NULL;
}
