//
//  model.hpp
//
//  Created by 罗 世通 on 2019/3/28.
//

#ifndef model_hpp
#define model_hpp

#include "base.hpp"

using namespace wglfw;


namespace playground {
    
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };
    
    class Model : public Base {
    public:
        static int main() {
            
            std::string vshader_path = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/model/model.vert";
            std::string fshader_path = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/model/model.frag";
            std::string model_path = "/Users/tommyluo/workspace/Project/OpenGLTest/Models/nanosuit/nanosuit.obj";
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            
            Program * prog = simple_shader_program(vshader_path, fshader_path);
            
            DefaultMeshTextureNameGeneratorFactory * name_gen_factory = new DefaultMeshTextureNameGeneratorFactory();
            AssimpModel * model = AssimpModel::fromFile(model_path.c_str(), name_gen_factory);
            
            CleanerCollection * cleaner = new CleanerCollection;
            cleaner->add(new ColorBufferCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)))->add(new DepthBufferCleaner);
            
            while (!window->shouldClose())
            {
                stopwatch();
                processInput(window);
                GL::clear(cleaner);
                
                prog->use();
                setProjectionViewMatrixOfProgram(prog);
                
                glm::mat4 model_mat = glm::mat4(1.0f);
                model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.75f, 0.0f));
                model_mat = glm::scale(model_mat, glm::vec3(0.2f, 0.2f, 0.2f));
                prog->setMatrix4("model", model_mat);
                model->draw(prog);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
        
    };
}

#endif /* model_hpp */
