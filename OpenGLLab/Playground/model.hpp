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

    class Model : public Base {
    public:
        static int main() {

            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            
            Program * prog = simple_shader_program(shader_path("model/model.vert"), shader_path("model/model.frag"));
            
            DefaultMeshTextureNameGeneratorFactory * name_gen_factory = new DefaultMeshTextureNameGeneratorFactory();
            AssimpModel * model = AssimpModel::fromFile(model_path("85-cottage_obj/cottage_obj.obj").c_str(), name_gen_factory);
            
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
