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
#include <cstddef>
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../GL.hpp"

namespace wglfw {
    
    struct MeshVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinate;
    };
    
    struct MeshTexture {
        std::string name;
        Texture * texture;
    };
    
    class Mesh {
    private:
        VertexArray * VAO;
        ArrayBuffer * VBO;
        ElementArrayBuffer * EBO;
        bool isSetUp;
    public:
        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture> textures;
        
        Mesh(std::vector<MeshVertex> & v, std::vector<unsigned int> & i, std::vector<MeshTexture> & t): vertices(v), indices(i), textures(t) {
            VAO = NULL;
            VBO = NULL;
            EBO = NULL;
        }
        
        ~Mesh() {
            if (VAO) delete VAO;
            if (VBO) delete VBO;
            if (EBO) delete EBO;
        }
        
        Mesh * setUp() {
            VAO = new VertexArray();
            VBO = new ArrayBuffer();
            EBO = new ElementArrayBuffer();
            
            VAO->bind();
            VBO->bind()->load(vertices.size()*sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);
            EBO->bind()->load(indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
         
            VertexAttributePointerConfiguration * config;
            VertexAttributePointer * pointerObject;
            
            config = VertexAttributePointerConfiguration::make()
                        ->index(0)
                        ->size(3)
                        ->type(GL_FLOAT)
                        ->normalized(GL_FALSE)
                        ->stride(sizeof(MeshVertex))
                        ->pointer((void *)0);
            pointerObject = VBO->getVertexAttributePointer(config);
            pointerObject->enable();
            delete config;
            delete pointerObject;
            
            config = VertexAttributePointerConfiguration::make()
                        ->index(1)
                        ->size(3)
                        ->type(GL_FLOAT)
                        ->normalized(GL_FALSE)
                        ->stride(sizeof(MeshVertex))
                        ->pointer((void *)offsetof(MeshVertex, normal));
            pointerObject = VBO->getVertexAttributePointer(config);
            pointerObject->enable();
            delete config;
            delete pointerObject;
            
            config = VertexAttributePointerConfiguration::make()
                        ->index(2)
                        ->size(2)
                        ->type(GL_FLOAT)
                        ->normalized(GL_FALSE)
                        ->stride(sizeof(MeshVertex))
                        ->pointer((void *)offsetof(MeshVertex, textureCoordinate));
            pointerObject = VBO->getVertexAttributePointer(config);
            pointerObject->enable();
            delete config;
            delete pointerObject;
        
            VBO->unbind();
            EBO->unbind();
            
            VAO->unbind();
            VertexArray::resumePreviousBinding();
            
            return this;
        }
        
        Mesh * draw(Program * program) {
            for (int i = 0; i < textures.size(); ++ i) {
                (textures[i]).texture->bindToTextureUnit(TextureUnit::get(i));
                program->setTexture(textures[i].name.c_str(), TextureUnit::get(i));
            }
            Texture::activateTextureUnit(TextureUnit::get(0));
            VAO->bind();
            GL::drawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            VertexArray::resumePreviousBinding();
            return this;
        }
        
        
    };
    
}

#endif /* Mesh_hpp */
