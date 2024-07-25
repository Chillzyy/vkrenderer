
![Screenshot_20240725_102842](https://github.com/user-attachments/assets/7a759d6e-76f9-437c-bb06-84b322695634)

Dependencies:

  GLFW: 
  https://github.com/glfw/glfw/
  
  Vulkan (headers, obv the drivers too): 
  https://vulkan.lunarg.com/sdk/home
  
  CGLM: 
  https://github.com/recp/cglm

  glslc (Shader Compiler):
  https://github.com/google/shaderc/blob/main/downloads.md

  (Don't forget to move glslc into the libs folder (or change the CMake script to search for it in another location))
  
  You also might need to recompile the static libraries located in the libs folder, directions for doing that can be found here:
  
  https://www.glfw.org/docs/latest/compile.html
  
  https://xkbcommon.org/doc/current/

    
Note for Linux users:

  Most (if not all) of these (depending on your distro) can be installed through the terminal, or some terminal-based app


The goal of this project is to create a neat Vulkan abstraction in C, for whoever may need it, while at the same time, improving my own Vulkan (and C) skills


Do whatever you want with this project I truly do not care
