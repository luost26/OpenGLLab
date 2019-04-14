//
//  Mesh.hpp
//
//  Created by 罗 世通 on 2019/3/28.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "../support.hpp"
#include "../VertexArray.hpp"
#include "../Texture.hpp"
#include "../GL.hpp"
#include "../Exception.hpp"

namespace wglfw {
    
    class MeshNotSetupException : public Exception {
        const char * what () const throw () {
            return "MeshNotSetupException";
        }
    };
    
    struct MeshVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinate;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
    
    struct MeshTexture {
        std::string name;
        Texture * texture;
        MeshTexture(const std::string & n, Texture * t): name(n), texture(t) {}
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
            isSetUp = false;
        }
        
        ~Mesh() {
            if (VAO) delete VAO;
            if (VBO) delete VBO;
            if (EBO) delete EBO;
        }
        
        Mesh * setUp() {
            if (isSetUp) {
                return this;
            }
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
            
            config = VertexAttributePointerConfiguration::make()
                        ->index(3)
                        ->size(3)
                        ->type(GL_FLOAT)
                        ->normalized(GL_FALSE)
                        ->stride(sizeof(MeshVertex))
                        ->pointer((void *)offsetof(MeshVertex, tangent));
            pointerObject = VBO->getVertexAttributePointer(config);
            pointerObject->enable();
            delete config;
            delete pointerObject;
            
            config = VertexAttributePointerConfiguration::make()
                        ->index(4)
                        ->size(3)
                        ->type(GL_FLOAT)
                        ->normalized(GL_FALSE)
                        ->stride(sizeof(MeshVertex))
                        ->pointer((void *)offsetof(MeshVertex, bitangent));
            pointerObject = VBO->getVertexAttributePointer(config);
            pointerObject->enable();
            delete config;
            delete pointerObject;
        
//            VAO->unbind();
            VertexArray::resumePreviousBinding();
//            VBO->unbind(); do not unbind VBO and EBO
//            EBO->unbind();
            
            isSetUp = true;
            
            return this;
        }
        
        Mesh * draw(Program * program) {
            if (!isSetUp) {
                throw MeshNotSetupException();
            }
            
            for (int i = 0; i < textures.size(); ++ i) {
                (textures[i]).texture->bindToTextureUnit(TextureUnit::get(i));
                program->setTexture(textures[i].name.c_str(), TextureUnit::get(i));
            }
            Texture::activateTextureUnit(TextureUnit::get(0));
            VAO->bind();
            GL::drawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
            VertexArray::resumePreviousBinding();
            return this;
        }
        
        
    };
    
}

#endif /* Mesh_hpp */
