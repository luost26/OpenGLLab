//
//  Floor.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef Floor_hpp
#define Floor_hpp

#include "../../../WrappedGLFW/loader.hpp"
#include "PointLight.hpp"
#include <string.h>

namespace playground {
    
    class Floor {
    private:
        float vertices[6*5];
        VertexArray * VAO;
        ArrayBuffer * VBO;
        
        Texture *colorTex, *normTex, *occTex, *roughTex;
    public:
        Floor(float ts = 1.0f) {
            colorTex = NULL;
            normTex = NULL;
            occTex = NULL;
            roughTex = NULL;
            float v[6*5] = {
                1.0f, 0.0f, 1.0f, ts, 0.0f,
                -1.0f, 0.0f, -1.0f, 0.0f, ts,
                -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 1.0f, ts, 0.0f,
                1.0f, 0.0f, -1.0f, ts, ts,
                -1.0f, 0.0f, -1.0f, 0.0f, ts
            };
            memcpy(vertices, v, sizeof(vertices));
            
            VBO = new ArrayBuffer();
            VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            VAO = new VertexArray();
            VAO->bind();
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
            VertexArray::resumePreviousBinding();
        }
        
        Floor * colorTexture(Texture * t) {
            colorTex = t;
            return this;
        }
        
        Floor * occlusionTexture(Texture * t) {
            occTex = t;
            return this;
        }
        
        Floor * normalTexture(Texture * t) {
            normTex = t;
            return this;
        }
        
        Floor * roughTexture(Texture * t) {
            roughTex = t;
            return this;
        }
        
        void draw(Program * prog, glm::mat4 model=glm::mat4(1.0f)) {
            if (!prog->isUsing()) {
                prog->use();
            }
            
            if (colorTex) colorTex->bindToTextureUnit(TextureUnit::get(0));
            if (normTex)  normTex->bindToTextureUnit(TextureUnit::get(1));
            if (occTex)   occTex->bindToTextureUnit(TextureUnit::get(2));
            if (roughTex) roughTex->bindToTextureUnit(TextureUnit::get(3));
            
            prog->setMatrix4("model", model)
                ->setTexture("color_texture", TextureUnit::get(0))
                ->setTexture("norm_texture", TextureUnit::get(1))
                ->setTexture("occ_texture", TextureUnit::get(2))
                ->setTexture("rough_texture", TextureUnit::get(3));
            
            VAO->bind();
            GL::drawArrays(GL_TRIANGLES, 0, 6);
        }
        
        ~Floor() {
            delete VAO;
            delete VBO;
        }
    };
    
}

#endif /* Floor_hpp */
