//
//  framebuffer.hpp
//
//  Created by 罗 世通 on 2019/4/1.
//

#ifndef framebuffer_hpp
#define framebuffer_hpp

#include "base.hpp"
#include "Objects/Box.hpp"
#include "Objects/PointLight.hpp"
#include "Objects/Floor.hpp"
#include "Objects/Grass.hpp"

namespace playground {
    
    class Framebuffer : public Base {
    public:
        static int main() {
            
            std::string color_tex = texture_path("Wood_Floor_007_SD/Wood_Floor_007_COLOR.jpg");
            std::string norm_tex  = texture_path("Wood_Floor_007_SD/Wood_Floor_007_NORM.jpg");
            std::string occ_tex   = texture_path("Wood_Floor_007_SD/Wood_Floor_007_OCC.jpg");
            std::string rough_tex = texture_path("Wood_Floor_007_SD/Wood_Floor_007_ROUGH.jpg");
            
            std::string vs = shader_path("box/box.vert");
            std::string fs = shader_path("box/box.frag");
            std::string di = texture_path("container2.png");
            std::string sp = texture_path("lighting_maps_specular_color.png");
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            glEnable(GL_CULL_FACE);
            
            Program * floor_prog = simple_shader_program(shader_path("floor/floor.vert"), shader_path("floor/floor.frag"));
            
            float fscale = 10.0f;
            Floor * floor = new Floor(fscale);
            floor->colorTexture(simple_texture(color_tex))
                    ->normalTexture(simple_texture(norm_tex))
                    ->occlusionTexture(simple_texture(occ_tex))
                    ->roughTexture(simple_texture(rough_tex));
            
            
            Program * box_prog = simple_shader_program(vs, fs);
            Texture * tex_diff = simple_texture(di);
            Texture * tex_spec = simple_texture(sp);
            Box * box = new Box(tex_diff, tex_spec);
            
            PointLight * light = PointLight::create()->position(glm::vec3(0.0f, 1.0f, 0.0f))->color(glm::vec3(1.0,1.0,1.0))->attenuation();
            

            
            
            FrameBuffer * FBO = new FrameBuffer();
            FBO->bind();

            Texture2D * frame_tex = new Texture2D();
            frame_tex->bind()->empty(realScreenSize(800), realScreenSize(600), GL_RGB)
                ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR);
            FBO->attachTexture2D(frame_tex, GL_COLOR_ATTACHMENT0);

            RenderBuffer * RBO = new RenderBuffer();
            RBO->bind()->storage(realScreenSize(800), realScreenSize(600), GL_RGBA);
            FBO->attachRenderBuffer(RBO);

            FBO->assertIsComplete();
            FBO->unbind();
            
            
           
            
            
            float fvertices[] = {
                -1.0f,  1.0f,0,  0.0f, 1.0f,
                -1.0f, -1.0f,0,  0.0f, 0.0f,
                1.0f, -1.0f,0,  1.0f, 0.0f,
                
                -1.0f, 1.0f,0,  0.0f, 1.0f,
                1.0f, -1.0f,0,  1.0f, 0.0f,
                1.0f,  1.0f,0,  1.0f, 1.0f
            };
            
            Program * frameProg = simple_shader_program(shader_path("framebuffer/fb.vert"), shader_path("framebuffer/fb.frag"));
            
            VertexArray * frameVAO = new VertexArray();
            frameVAO->bind();
            
            ArrayBuffer * frameVBO = new ArrayBuffer();
            frameVBO->bind()->load(sizeof(fvertices), fvertices, GL_STATIC_DRAW);
            frameVBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(0)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer(0)
                                           )->enable();
            frameVBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(1)
                                           ->size(2)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer((void *)(3*sizeof(float)))
                                           )->enable();
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                
                FBO->bind();
                glEnable(GL_DEPTH_TEST);
                
                GL::clear(getDefaultCleanerCollection());  // Must be done after binding FBO
                
                floor_prog->use();
                setProjectionViewMatrixOfProgram(floor_prog);
                light->apply(floor_prog, cam->position() ,"point_light", "viewPos");
                floor->draw(floor_prog, scale(translate(mat4(1.0), vec3(0, -0.5, 0)),vec3(fscale, fscale, fscale)) );
                
                box_prog->use();
                setProjectionViewMatrixOfProgram(box_prog);
                light->apply(box_prog, cam->position(),"pointLight", "viewPos");
                box->draw(box_prog, translate(mat4(1.0), vec3(-2.0, 0.0, -1.0)));
                
                FBO->unbind();
                glDisable(GL_DEPTH_TEST);

                glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            
                frameProg->use();
                frame_tex->bindToTextureUnit(TextureUnit::get(0));
                frameProg->setTexture("tex", TextureUnit::get(0));
                frameVAO->bind();
                
                GL::drawArrays(GL_TRIANGLES, 0, 6);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            return 0;
        }
    };
    
}

#endif /* framebuffer_hpp */
