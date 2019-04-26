cmake_minimum_required(VERSION 3.12)
project(OpenGLLab)

set(CMAKE_CXX_STANDARD 14)

include_directories(
				glad/include
				include
				)

add_executable(OpenGLLab
        glad/src/glad.c
        OpenGLLab/main.cpp
)

add_definitions(
		-DTEXTURE_DIR="${PROJECT_SOURCE_DIR}/Textures"
		-DSHADER_DIR="${PROJECT_SOURCE_DIR}/Shaders"
		-DMODEL_DIR="${PROJECT_SOURCE_DIR}/Models"
	)

if (APPLE)
	set(VIEWPORT_SCALE_FACTOR 2)
else()
	set(VIEWPORT_SCALE_FACTOR 1)
endif()

add_definitions(-DVIEWPORT_SCALE_FACTOR=${VIEWPORT_SCALE_FACTOR})

if (APPLE)
    include_directories(/usr/local/include)
    target_link_libraries(OpenGLLab
            /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib
            /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.2.dylib
            "-framework CoreVideo"
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            )
elseif(WIN32)
	include_directories(
		"C:/Users/luost/workspace/Libraries/glfw-3.3.bin.WIN64/include"
		"C:/Users/luost/workspace/Libraries/glm"
		"C:/Program Files/Assimp/include"
	)
	target_link_libraries(OpenGLLab
		"C:/Users/luost/workspace/Libraries/glfw-3.3.bin.WIN64/lib-vc2017/glfw3.lib"
		"C:/Program Files/Assimp/lib/x64/assimp-vc140-mt.lib"
	)
	add_custom_command(TARGET OpenGLLab POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		"C:/Program Files/Assimp/bin/x64/assimp-vc140-mt.dll"
		$<TARGET_FILE_DIR:OpenGLLab>
	)

endif()