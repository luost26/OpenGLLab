//
//  geometry_shader.hpp
//
//  Created by 罗 世通 on 2019/4/11.
//

#ifndef geometry_shader_hpp
#define geometry_shader_hpp

#include "base.hpp"

namespace playground {
    
    class GeometryShader : public Base {
    public:
        static int main() {
            
            const float vertices[] = {
                0.5, 0.5,   1.0f, 0.0f, 0.0f,
                -0.5, -0.5, 0.0f, 1.0f, 0.0f,
                0.5, -0.5,  0.0f, 0.0f, 1.0f,
                -0.5, 0.5,  1.0f, 1.0f, 0.0f
            };
            
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            GL::enable(GL_PROGRAM_POINT_SIZE);
            GL::enable(GL_CULL_FACE);
            
            VertexArray * VAO = new VertexArray;
            ArrayBuffer * VBO = new ArrayBuffer;
            
            VAO->bind();
            VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(0)
                                           ->size(2)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer(0))->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(1)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(5*sizeof(float))
                                           ->pointer((void*)(2*sizeof(float))))->enable();
            
            VAO->unbind();
            
            AssimpModel * model = AssimpModel::fromFile(model_path("nanosuit/nanosuit.obj").c_str());
            
            UniformBuffer * cameraUBO = new UniformBuffer();
            cameraUBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();
            
            Program * house_prog = simple_shader_program(shader_path("gshader/gshader.vert"), shader_path("gshader/gshader.frag"), shader_path("gshader/gshader.geom"));
            Program * model_prog = simple_shader_program(shader_path("gshader/model.vert"), shader_path("gshader/model.frag"));
            Program * normal_prog = simple_shader_program(shader_path("gshader/normal.vert"), shader_path("gshader/normal.frag"), shader_path("gshader/normal.geom"));
            
            model_prog->setUniformBlockBinding("Camera", 0);
            normal_prog->setUniformBlockBinding("Camera", 0);
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                house_prog->use();
                VAO->bind();
                GL::drawArrays(GL_POINTS, 0, 4);
                
                cameraUBO->bind()->loadSub(0, sizeof(glm::mat4), glm::value_ptr(getCamera()->projectionMatrix()))->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getCamera()->viewMatrix()))->unbind();
                
                model_prog->use()->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
                model->draw(model_prog);
                
                normal_prog->use()->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
                model->draw(normal_prog);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
}

#endif /* geometry_shader_hpp */
