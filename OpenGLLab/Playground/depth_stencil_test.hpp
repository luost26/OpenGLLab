//
//  depth_stencil_test.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef depth_stencil_test_hpp
#define depth_stencil_test_hpp

#include "base.hpp"
#include "Objects/Box.hpp"
#include "Objects/PointLight.hpp"

using namespace wglfw;
using namespace glm;

namespace playground {
    
    class DepthStencilTest : public Base {
    public:
        static Program * prog, * normalProgram, * depthTestProgram, * stencilTestProgram;
        static void processInput(Window * window) {
            Base::processInput(window);
            if (window->getKey(GLFW_KEY_Z) == GLFW_PRESS)
                prog = normalProgram;
            if (window->getKey(GLFW_KEY_X) == GLFW_PRESS)
                prog = depthTestProgram;
            
            if (window->getKey(GLFW_KEY_L) == GLFW_PRESS)
                glDepthFunc(GL_LESS);
            if (window->getKey(GLFW_KEY_K) == GLFW_PRESS)
                glDepthFunc(GL_ALWAYS);
        }
        
        static int main() {
            std::string vs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/box.vert";
            std::string fs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/box.frag";
            std::string dtfs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/depth_test.frag";
            std::string stfs = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/box/single_color.frag";
            
            std::string di = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/container2.png";
            std::string sp = "/Users/tommyluo/workspace/Project/OpenGLTest/Textures/lighting_maps_specular_color.png";
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();

            GL::enable(GL_STENCIL_TEST);
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            
            normalProgram = simple_shader_program(vs, fs);
            depthTestProgram = simple_shader_program(vs, dtfs);
            stencilTestProgram = simple_shader_program(vs, stfs);
            prog = normalProgram;
            
            CleanerCollection * cleaner = new CleanerCollection;
            cleaner->add(new ColorBufferCleaner(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)))
                    ->add(new DepthBufferCleaner)
                    ->add(new StencilBufferCleaner);
            
            Texture * tex_diff = simple_texture(di);
            Texture * tex_spec = simple_texture(sp);
            Box * box1 = new Box(tex_diff, tex_spec);
            Box * box2 = new Box(tex_diff, tex_spec);
            PointLight * light = PointLight::create()->position(glm::vec3(3.0f, 3.0f, 3.0f))->color(glm::vec3(1.0,1.0,1.0));
            
            while (!window->shouldClose())
            {
                stopwatch();
                processInput(window);
                GL::clear(cleaner);
                
                prog->use();
                setProjectionViewMatrixOfProgram(prog);
                light->apply(prog, cam->position() ,"pointLight", "viewPos");
                
                
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
                box1->draw(prog);
                box2->draw(prog, translate(mat4(1.0), vec3(-0.8, 0.0, -1.5)));
                
                
                stencilTestProgram->use();
                setProjectionViewMatrixOfProgram(stencilTestProgram);
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);
                box1->draw(stencilTestProgram, scale(mat4(1.0), vec3(1.1, 1.1, 1.1)) );
                box2->draw(stencilTestProgram, scale(translate(mat4(1.0), vec3(-0.8, 0.0, -1.5)), vec3(1.1, 1.1, 1.1)) );
                glStencilMask(0xFF);
                glEnable(GL_DEPTH_TEST);
                
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
    Program * DepthStencilTest::prog = NULL;
    Program * DepthStencilTest::normalProgram = NULL;
    Program * DepthStencilTest::depthTestProgram = NULL;
    Program * DepthStencilTest::stencilTestProgram = NULL;
    
}
#endif /* depth_stencil_test_hpp */
