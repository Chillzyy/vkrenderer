#include "gfx/vk/vk_window.h"

bool framebuffer_resized = false;

static void glfw_error_callback(int code, const char *description)
{
    printf("GLFW Error %d: %s\n", code, description);
}

static void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    // Doing this to avoid unused parameter errors
    void *trash = glfwGetWindowUserPointer(window);
    framebuffer_resized = true;
    printf("New window size: %d x %d\n", width, height);
    printf("Window pointer address: %p\n", trash);
}

GLFWwindow *vk_window_init(const char *title)
{
    const uint32_t WINDOW_WIDTH = 800;
    const uint32_t WINDOW_HEIGHT = 600;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwSetErrorCallback(glfw_error_callback);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);

    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);

    if (window == NULL)
    {
        printf("Failed to create window\n");
    }

    return window;
}

void vk_window_destroy(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    window = NULL;
}
