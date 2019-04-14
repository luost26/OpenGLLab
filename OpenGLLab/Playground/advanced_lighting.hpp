//
//  advanced_lighting.hpp
//
//  Created by 罗 世通 on 2019/4/14.
//

#ifndef advanced_lighting_hpp
#define advanced_lighting_hpp

#include "base.hpp"

namespace playground {
    
    class AdvancedLighting : public Base {
    public:
        static int main() {
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            
            UniformBuffer * cameraUBO = new UniformBuffer();
            cameraUBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();
            
            Program * floor_prog = simple_shader_program(shader_path("advanced_lighting/floor.vert"), shader_path("advanced_lighting/floor.frag"));
            floor_prog->setUniformBlockBinding("Camera", 0);
            
            Floor * floor = new Floor(10.0f);
            floor->colorTexture(simple_texture(texture_path("wood.png")));
            
            PointLight * light = PointLight::create()->position(glm::vec3(0.0, 1.0, 0.0))->color(glm::vec3(1.0, 1.0, 1.0), 0.05, 1.0, 0.3);
            
            getCamera()->moveUp(0.5);
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                cameraUBO->bind()->loadSub(0, sizeof(glm::mat4), glm::value_ptr(getCamera()->projectionMatrix()))->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getCamera()->viewMatrix()))->unbind();
                
                floor_prog->use();
                light->apply(floor_prog, getCamera()->position(), "light", "viewPos");
                floor->draw(floor_prog, glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
                
            
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
}

#endif /* advanced_lighting_hpp */
