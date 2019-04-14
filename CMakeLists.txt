cmake_minimum_required(VERSION 3.13)
project(OpenGLLab)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
include_directories(glad/include)
include_directories(include)
include_directories(WrappedGLFW)
include_directories(OpenGLLab/Playground)

add_executable(OpenGLLab
        glad/src/glad.c
        OpenGLLab/main.cpp
)

target_link_libraries(OpenGLLab
        /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib
        /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.2.dylib
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        )