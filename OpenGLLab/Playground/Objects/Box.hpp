//
//  Box.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef Box_hpp
#define Box_hpp

#include "../../../WrappedGLFW/loader.hpp"
#include "PointLight.hpp"

#include <vector>

namespace playground {
    
    class Box {
    private:
        static float vertices[6*6*8];
        static ArrayBuffer * VBO;
        static glm::mat4 defaultModel;
        
        static VertexArray * VAO;
        Texture * textureDiffuse;
        Texture * textureSpecular;
        
        static void init() {
            VBO = new ArrayBuffer();
            VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            VAO = new VertexArray();
            VAO->bind();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(0)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer(0)
                                           )->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(1)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer((void *)(3*sizeof(float)))
                                           )->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(2)
                                           ->size(2)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer((void *)(6*sizeof(float)))
                                           )->enable();
            VertexArray::resumePreviousBinding();
            
        }
    public:
        Box(Texture * tex, Texture * spTex) {
            if (VBO == NULL) {
                init();
            }
            textureDiffuse = tex;
            textureSpecular = spTex;
        }
                
        void draw(Program * prog, glm::mat4 model=Box::defaultModel) {
            if (!prog->isUsing()) {
                prog->use();
            }
            
            if (textureDiffuse) {
                textureDiffuse->bindToTextureUnit(TextureUnit::get(0));
                prog->setTexture("material.texture_diffuse0", TextureUnit::get(0));
            }
            
            if (textureSpecular) {
                textureSpecular->bindToTextureUnit(TextureUnit::get(1));
                prog->setTexture("material.texture_specular0", TextureUnit::get(1))
                ->setFloat("material.shininess", 32);
            }
                        
            prog->setMatrix4("model", model);
            
            VAO->bind();
            
            GL::drawArrays(GL_TRIANGLES, 0, 36);
        }
        
    };
    
    glm::mat4 Box::defaultModel = glm::mat4(1.0f);
    VertexArray * Box::VAO = NULL;
    ArrayBuffer * Box::VBO = NULL;
    float Box::vertices[6*6*8] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
}

#endif /* Box_hpp */
