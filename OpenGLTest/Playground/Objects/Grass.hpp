//
//  Grass.hpp
//
//  Created by 罗 世通 on 2019/4/1.
//

#ifndef Grass_hpp
#define Grass_hpp

#include "../../../WrappedGLFW/loader.hpp"

using namespace wglfw;

namespace playground {
    
    class Grass {
    public:
        static VertexArray * VAO;
        static ArrayBuffer * VBO;
        static ElementArrayBuffer * EBO;
        static float vertices[20];
        static int elements[6];
        
        Texture * tex;
        
        Grass(Texture * t) {
            if (VAO != NULL && VBO != NULL)
                return;
            
            VAO = new VertexArray();
            VAO->bind();
            
            VBO = new ArrayBuffer();
            VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            
            EBO = new ElementArrayBuffer();
            EBO->bind()->load(sizeof(elements), elements, GL_STATIC_DRAW);
            
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(0)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer(0)
                                           )->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(1)
                                           ->size(2)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer((void *)(3*sizeof(float)))
                                           )->enable();
            tex = t;
        }
        
        void draw(Program * prog, glm::mat4 model = glm::mat4(1.0f)) {
            if (!prog->isUsing()) {
                prog->use();
            }
            
            tex->bindToTextureUnit(TextureUnit::get(0));
            
            prog->setMatrix4("model", model)
                ->setTexture("grass_texture", TextureUnit::get(0));
            
            VAO->bind();
            
            GL::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    };
    
    VertexArray * Grass::VAO = NULL;
    ArrayBuffer * Grass::VBO = NULL;
    ElementArrayBuffer * Grass::EBO = NULL;
    
    float Grass::vertices[20] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 0.0, 1.0
    };
    
    int Grass::elements[6] = {
        0, 1, 3, 1, 2, 3
    };
    
}

#endif /* Grass_hpp */
