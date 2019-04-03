//
//  blending.hpp
//
//  Created by 罗 世通 on 2019/4/1.
//

#ifndef blending_hpp
#define blending_hpp


#include "base.hpp"
#include "Objects/Box.hpp"
#include "Objects/PointLight.hpp"
#include "Objects/Floor.hpp"
#include "Objects/Grass.hpp"

using namespace wglfw;
using namespace glm;

namespace playground {
    
    class Blending : public Base {
    public:
        static int main() {
            
            const char * floor_vert = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/floor/floor.vert";
            const char * floor_frag = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/floor/floor.frag";
            
            const char * color_tex = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/Wood_Floor_007_SD/Wood_Floor_007_COLOR.jpg";
            const char * norm_tex  = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/Wood_Floor_007_SD/Wood_Floor_007_NORM.jpg";
            const char * occ_tex   = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/Wood_Floor_007_SD/Wood_Floor_007_OCC.jpg";
            const char * rough_tex = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/Wood_Floor_007_SD/Wood_Floor_007_ROUGH.jpg";
            
            const char * vs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/box.vert";
            const char * fs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/box.frag";
            const char * di = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/container2.png";
            const char * sp = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/lighting_maps_specular_color.png";
            
            const char * grass_v = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/etc/grass.vert";
            const char * grass_f = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/etc/grass.frag";
            const char * grass_t = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/grass.png";
            
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            GL::enable(GL_BLEND);
            
            Program * floor_shader = simple_shader_program(floor_vert, floor_frag);
            Program * box_shader = simple_shader_program(vs, fs);
            
            float floor_scale = 10.0f;
            Floor * floor = new Floor(floor_scale);
            floor->colorTexture(simple_texture(color_tex))
                    ->normalTexture(simple_texture(norm_tex))
                    ->occlusionTexture(simple_texture(occ_tex))
                    ->roughTexture(simple_texture(rough_tex));
            
            Texture * tex_diff = simple_texture(di);
            Texture * tex_spec = simple_texture(sp);
            Box * box1 = new Box(tex_diff, tex_spec);
            
            PointLight * light = PointLight::create()->position(glm::vec3(0.0f, 1.0f, 0.0f))->color(glm::vec3(1.0,1.0,1.0))->attenuation();
            
            Texture * tex_grass = simple_texture(grass_t, GL_CLAMP_TO_EDGE);
            Program * grass_shader = simple_shader_program(grass_v, grass_f);
            Grass * grass = new Grass(tex_grass);
            
            while (!window->shouldClose()) {
                
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                floor_shader->use();
                setProjectionViewMatrixOfProgram(floor_shader);
                light->apply(floor_shader, cam->position() ,"point_light", "viewPos");
                floor->draw(floor_shader, scale(translate(mat4(1.0), vec3(0, -0.5, 0)),vec3(floor_scale, floor_scale, floor_scale)) );

                box_shader->use();
                setProjectionViewMatrixOfProgram(box_shader);
                light->apply(box_shader, cam->position() ,"pointLight", "viewPos");
                box1->draw(box_shader, translate(mat4(1.0), vec3(-2, 0.0, -2)));
                
                grass_shader->use();
                setProjectionViewMatrixOfProgram(grass_shader);
                grass->draw(grass_shader, translate(mat4(1.0), vec3(-2.0, -0.5, -1.0)));
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
                
            }
            
            return 0;
        }
        
    };
    
}

#endif /* blending_hpp */
