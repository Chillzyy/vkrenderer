#ifndef vk_window_h
#define vk_window_h

#include "GLFW/glfw3.h"
#include <stdio.h>
#include <stdbool.h>

extern bool framebuffer_resized;

GLFWwindow *vk_window_init(const char *title);
void vk_window_destroy(GLFWwindow *window);

#endif
