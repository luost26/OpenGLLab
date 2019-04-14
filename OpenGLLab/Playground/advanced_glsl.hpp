//
//  advanced_glsl.hpp
//
//  Created by 罗 世通 on 2019/4/11.
//

#ifndef advanced_glsl_hpp
#define advanced_glsl_hpp

#include "base.hpp"
#include "Objects/Box.hpp"

namespace playground {
    class AdvancedGLSL : public Base {
    public:
        static int main() {
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            
            Program * red_prog = simple_shader_program(shader_path("advanced/box.vert"), shader_path("advanced/red.frag"));
            Program * yellow_prog = simple_shader_program(shader_path("advanced/box.vert"), shader_path("advanced/yellow.frag"));
            Box * box = new Box(NULL, NULL);
            
            red_prog->setUniformBlockBinding("Matrices", 0);
            red_prog->setUniformBlockBinding("Matrices", 0);
            
            Camera * cam = getCamera();
            
            UniformBuffer * UBO = new UniformBuffer();
            UBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();
            
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                UBO->bind()->loadSub(0, sizeof(glm::mat4), glm::value_ptr(cam->projectionMatrix()))->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam->viewMatrix()))->unbind();
                
                box->draw(red_prog);
                box->draw(yellow_prog, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f)));
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
}

#endif /* advanced_glsl_hpp */
