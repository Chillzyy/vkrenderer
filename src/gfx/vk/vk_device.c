#include "gfx/vk/vk_device.h"

static VkDeviceQueueCreateInfo vk_device_queue_info_init(float *queue_priority, uint32_t queue_count, uint32_t queue_index)
{
    VkDeviceQueueCreateInfo queue_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueFamilyIndex = queue_index,
        .queueCount = queue_count,
        .pQueuePriorities = queue_priority
    };

    return queue_info;
}

static VkDeviceCreateInfo vk_device_info_init(VkDeviceQueueCreateInfo *device_queue_infos, uint32_t device_queue_info_count, const char **extensions, const uint32_t extension_count)
{
    VkDeviceCreateInfo device_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pQueueCreateInfos = device_queue_infos,
        .queueCreateInfoCount = device_queue_info_count,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions,
        .pEnabledFeatures = NULL
    };

    return device_info;
}

vk_device vk_device_init(const char *title, bool debugging)
{
    VkResult result = VK_SUCCESS;
    VkDevice device = {0};
    VkDebugUtilsMessengerEXT debug_messenger = {0};

    GLFWwindow *window = vk_window_init(title);
    VkInstance instance = vk_instance_init(title, debugging);

    if (debugging)
    {
        debug_messenger = vk_debug_messenger_init(instance);
    }

    VkSurfaceKHR surface = vk_surface_init(instance, window);

    VkPhysicalDevice gpu = vk_gpu_init(instance, surface);

    queue_family_indices indices = vk_queue_families_init(gpu, surface);

    float queue_priority = 1.0f;
    uint32_t queue_count = 1;
    uint32_t queue_info_count = (indices.graphics.index != indices.present.index && indices.graphics.index != indices.transfer.index) ? 3 : 1;

    VkDeviceQueueCreateInfo graphics_queue_info = vk_device_queue_info_init(&queue_priority, queue_count, indices.graphics.index);
    VkDeviceQueueCreateInfo present_queue_info = vk_device_queue_info_init(&queue_priority, queue_count, indices.present.index);
    VkDeviceQueueCreateInfo transfer_queue_info = vk_device_queue_info_init(&queue_priority, queue_count, indices.transfer.index);
    VkDeviceQueueCreateInfo queue_infos[3] = {graphics_queue_info, present_queue_info, transfer_queue_info};

    VkDeviceCreateInfo device_info = vk_device_info_init(queue_infos, queue_info_count, DEVICE_EXTENSIONS, DEVICE_EXTENSION_COUNT);

    result = vkCreateDevice(gpu, &device_info, NULL, &device);

    if (result != VK_SUCCESS)
    {
        printf("Failed to create device\n");
    }

    vk_queues queues = vk_queues_init(device, indices);

    VkCommandPool graphics_cmd_pool = vk_cmd_pool_init(device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, indices.graphics.index);
    VkCommandPool transfer_cmd_pool = vk_cmd_pool_init(device, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, indices.transfer.index);

    vk_device d =
    {
        .window = window,
        .surface = surface,
        .instance = instance,
        .debug_messenger = debug_messenger,
        .gpu = gpu,
        .graphics_cmd_pool = graphics_cmd_pool,
        .transfer_cmd_pool = transfer_cmd_pool,
        .indices = indices,
        .queues = queues,
        .device = device
    };

    return d;
}

void vk_device_destroy(vk_device *d, bool debugging)
{
    vk_cmd_pool_destroy(d->device, d->transfer_cmd_pool);
    vk_cmd_pool_destroy(d->device, d->graphics_cmd_pool);

    vkDestroyDevice(d->device, NULL);

    if (debugging)
    {
        vk_debug_messenger_destroy(d->instance, d->debug_messenger, NULL);
    }

    vk_surface_destroy(d->instance, d->surface);
    vk_instance_destroy(d->instance);

    vk_window_destroy(d->window);

    d = NULL;
}
