//
//  support.hpp
//
//  Created by 罗 世通 on 2019/3/18.
//

#ifndef support_hpp
#define support_hpp

/* C++ Libraries */
#include <vector>
#include <string>
#include <map>
#include <algorithm>

/* Input/Output */
#include <fstream>
#include <sstream>
#include <iostream>

/* Misc. CPP Headers */
#include <memory>
#include <exception>
#include <cstring>
#include <cstddef>

/* GLAD & GLFW */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* STB IMAGE LIBRARY */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* GLM */
#define GLM_FORCE_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_aligned.hpp>

/* Assimp */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif /* support_hpp */
