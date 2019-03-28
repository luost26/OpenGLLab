//
//  Mesh.hpp
//
//  Created by 罗 世通 on 2019/3/28.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace wglfw {
    
    struct MeshVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinate;
    };
    
    struct MeshTexture {
        std::string type;
        unsigned int id;
    };
    
    class Mesh {
    public:
        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture> textures;
        
        
    };
    
}

#endif /* Mesh_hpp */
