cmake_minimum_required(VERSION 3.10)
project(renderer VERSION 0.1.0 DESCRIPTION "Vulkan Renderer")

# Project specs
set(C_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# https://discourse.cmake.org/t/how-to-automatically-add-header-and-source-files-to-executable-without-file-glob/395/2 :(
set(SOURCE_FILES
    src/main.c
    src/app/app.c
    src/gfx/vk/vk_context.c
    src/gfx/vk/vk_window.c
    src/gfx/vk/vk_instance.c
    src/gfx/vk/vk_ext.c
    src/gfx/vk/vk_debug.c
    src/gfx/vk/vk_gpu.c
    src/gfx/vk/vk_queue.c
    src/gfx/vk/vk_device.c
    src/gfx/vk/vk_surface.c
    src/gfx/vk/vk_swapchain.c
    src/gfx/vk/vk_img_views.c
    src/gfx/vk/vk_shaders.c
    src/gfx/vk/vk_pipeline.c
    src/gfx/vk/vk_pipeline_layout.c
    src/gfx/vk/vk_render_pass.c
    src/gfx/vk/vk_framebuffers.c
    src/gfx/vk/vk_cmd_pool.c
    src/gfx/vk/vk_cmd_buffers.c
    src/gfx/vk/vk_sync_obj.c
    src/gfx/vk/vk_present_img.c
    src/gfx/vk/vk_buffer.c
    src/gfx/vk/vk_vertex.c

    include/app/app.h
    include/gfx/vk/vk_context.h
    include/gfx/vk/vk_window.h
    include/gfx/vk/vk_instance.h
    include/gfx/vk/vk_ext.h
    include/gfx/vk/vk_debug.h
    include/gfx/vk/vk_gpu.h
    include/gfx/vk/vk_queue.h
    include/gfx/vk/vk_device.h
    include/gfx/vk/vk_surface.h
    include/gfx/vk/vk_swapchain.h
    include/gfx/vk/vk_img_views.h
    include/gfx/vk/vk_shaders.h
    include/gfx/vk/vk_pipeline.h
    include/gfx/vk/vk_pipeline_layout.h
    include/gfx/vk/vk_render_pass.h
    include/gfx/vk/vk_framebuffers.h
    include/gfx/vk/vk_cmd_pool.h
    include/gfx/vk/vk_cmd_buffers.h
    include/gfx/vk/vk_sync_obj.h
    include/gfx/vk/vk_present_img.h
    include/gfx/vk/vk_buffer.h
    include/gfx/vk/vk_vertex.h
)

# Find Vulkan (mandatory)
find_package(Vulkan REQUIRED)

# Linker options
add_executable(renderer ${SOURCE_FILES})
target_include_directories(renderer PRIVATE include)
target_link_directories(renderer PRIVATE lib)
target_link_directories(renderer PRIVATE src)
target_link_libraries(renderer ${CMAKE_SOURCE_DIR}/libs/libglfw3.a ${CMAKE_SOURCE_DIR}/libs/libxkbcommon.a pthread dl vulkan m)

# Find GLSL shaders
set(SHADER_PATH ${CMAKE_SOURCE_DIR}/shaders/)
file(GLOB SHADERS RELATIVE ${CMAKE_SOURCE_DIR} "${SHADER_PATH}*.vert" "${SHADER_PATH}*.frag" "${SHADER_PATH}*.comp")

# Compile each GLSL shader to SPIR-V
foreach(shader ${SHADERS})
	set(INPUT_GLSL "${CMAKE_SOURCE_DIR}/${shader}")
	set(OUTPUT_SPV "${INPUT_GLSL}.spv")
	# Allow CMake to access and use shader compiler
	add_custom_command(OUTPUT "${OUTPUT_SPV}"
	COMMAND "${CMAKE_SOURCE_DIR}/libs/glslc" "${INPUT_GLSL}" "-o" "${OUTPUT_SPV}"
	DEPENDS "${INPUT_GLSL}"
	VERBATIM)
	list(APPEND SHADER_BIN ${OUTPUT_SPV})
endforeach()

# Build shaders along with project
add_custom_target(SHADER_OUT ALL DEPENDS ${SHADER_BIN} VERBATIM)

# Compiler options
target_compile_options(renderer PRIVATE -g3 -Wall -Wextra -Werror -Wpedantic -fno-omit-frame-pointer -ferror-limit=0 -fsanitize=address,undefined,leak)

# Linker options
target_link_options(renderer PRIVATE -fno-omit-frame-pointer -ferror-limit=0 -fsanitize=address,undefined,leak)
