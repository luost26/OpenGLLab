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
        static AssimpModel * model;
        static glm::mat4 model_mat;
        static std::vector<AssimpModel*> models;
        static std::vector<glm::mat4> models_mat;

        static void processInput(Window *window) {
            Base::processInput(window);
            if (window->getKey(GLFW_KEY_1) == GLFW_PRESS) {
                model = models[0];
                model_mat = models_mat[0];
            }
            if (window->getKey(GLFW_KEY_2) == GLFW_PRESS) {
                model = models[1];
                model_mat = models_mat[1];
            }
            if (window->getKey(GLFW_KEY_3) == GLFW_PRESS) {
                model = models[2];
                model_mat = models_mat[2];
            }
        }

        static int main() {

            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();

            //glEnable(GL_CULL_FACE);
            //glCullFace(GL_BACK);
            
            Program * prog = simple_shader_program(shader_path("model/model.vert"), shader_path("model/model.frag"));

            models.push_back( AssimpModel::fromFile(model_path("nanosuit/nanosuit.obj").c_str()) );
            models_mat.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));

            models.push_back( AssimpModel::fromFile(model_path("classroom/classroom2.obj").c_str()) );
            models_mat.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));

            models.push_back( AssimpModel::fromFile(model_path("look3/look3.obj").c_str()) );
            models_mat.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f)));

            model = models[0];
            model_mat = models_mat[0];

            CleanerCollection * cleaner = new CleanerCollection;
            cleaner->add(new ColorBufferCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)))->add(new DepthBufferCleaner);

            getCamera()->moveUp(2.0f);

            while (!window->shouldClose())
            {
                stopwatch();
                processInput(window);
                GL::clear(cleaner);

                prog->use();
                setProjectionViewMatrixOfProgram(prog);

                prog->setMatrix4("model", model_mat);
                model->draw(prog);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
        
    };

    std::vector<AssimpModel*> Model::models;
    AssimpModel * Model::model;
    glm::mat4 Model::model_mat;
    std::vector<glm::mat4> Model::models_mat;
}

#endif /* model_hpp */
