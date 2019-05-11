cmake_minimum_required(VERSION 3.12)
project(OpenGLLab)

set(CMAKE_CXX_STANDARD 14)

include_directories(
				glad/include
				include
				)

add_executable(Playground
        glad/src/glad.c
        OpenGLLab/Playground/main.cpp
)

add_executable(Showcase
        glad/src/glad.c
        OpenGLLab/Showcase/main.cpp
)

target_compile_definitions(Playground
		PUBLIC -DTEXTURE_DIR="${PROJECT_SOURCE_DIR}/Textures"
		PUBLIC -DSHADER_DIR="${PROJECT_SOURCE_DIR}/Shaders"
		PUBLIC -DMODEL_DIR="${PROJECT_SOURCE_DIR}/Models"
	)

if (APPLE)
	set(VIEWPORT_SCALE_FACTOR 2)
else()
	set(VIEWPORT_SCALE_FACTOR 1)
endif()

add_definitions(-DVIEWPORT_SCALE_FACTOR=${VIEWPORT_SCALE_FACTOR})

if (APPLE)
    include_directories(/usr/local/include)

    target_link_libraries(Playground
            /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib
            /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.2.dylib
            "-framework CoreVideo"
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            )

	target_link_libraries(Showcase
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

	target_link_libraries(Playground
		"C:/Users/luost/workspace/Libraries/glfw-3.3.bin.WIN64/lib-vc2017/glfw3.lib"
		"C:/Program Files/Assimp/lib/x64/assimp-vc140-mt.lib"
	)

	target_link_libraries(Showcase
		"C:/Users/luost/workspace/Libraries/glfw-3.3.bin.WIN64/lib-vc2017/glfw3.lib"
		"C:/Program Files/Assimp/lib/x64/assimp-vc140-mt.lib"
	)

	add_custom_command(TARGET Playground PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"C:/Program Files/Assimp/bin/x64/assimp-vc140-mt.dll"
		$<TARGET_FILE_DIR:Playground>
	)

	add_custom_command(TARGET Showcase PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"C:/Program Files/Assimp/bin/x64/assimp-vc140-mt.dll"
		$<TARGET_FILE_DIR:Showcase>
	)

endif()

add_custom_command(TARGET Showcase PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/Models
		$<TARGET_FILE_DIR:Showcase>/Resources/Models
		)

add_custom_command(TARGET Showcase PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/Shaders/Showcase
		$<TARGET_FILE_DIR:Showcase>/Resources/Shaders
		)

add_custom_command(TARGET Showcase PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/Textures
		$<TARGET_FILE_DIR:Showcase>/Resources/Textures
		)