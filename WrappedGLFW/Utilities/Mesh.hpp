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
#include "Material.hpp"

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

    class Mesh {
    private:
        VertexArray * VAO;
        ArrayBuffer * VBO;
        ElementArrayBuffer * EBO;
        bool isSetUp;
    public:
        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;
        Material * material;

        Mesh(std::vector<MeshVertex> & v, std::vector<unsigned int> & i, Material * m): vertices(v), indices(i), material(m) {
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

            int tex_unit = 0;
            for (auto tex_pair : material->textures) {
                tex_pair.second->bindToTextureUnit(TextureUnit::get(tex_unit));
                program->setTexture(tex_pair.first.c_str(), TextureUnit::get(tex_unit));
				program->setFloat((tex_pair.first + "_enabled").c_str(), 1.0f);
                ++ tex_unit;
            }
            Texture::activateTextureUnit(TextureUnit::get(0)); // reset texture unit activation status

            for (auto color_pair : material->vector3s) {
                program->setVec3(color_pair.first.c_str(), color_pair.second);
            }

            for (auto float_pair : material->floats) {
                program->setFloat(float_pair.first.c_str(), float_pair.second);
            }

            VAO->bind();
            GL::drawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
            VertexArray::resumePreviousBinding();
            return this;
        }
        
        
    };
    
}

#endif /* Mesh_hpp */
