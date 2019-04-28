//
// Created by 罗 世通 on 2019-04-28.
//

#ifndef OPENGLLAB_SHADOW2_HPP
#define OPENGLLAB_SHADOW2_HPP

namespace playground {

    class Shadow2Scene {
    public:
        AssimpModel * model;
        PointLight * light;

        Shadow2Scene() {
            model = AssimpModel::fromFile(model_path("kitchen/kitchen.obj").c_str());
            light = PointLight::create()->position(glm::vec3(5.25f, 1.85f, -1.6f))
                    ->color(glm::vec3(1.0, 1.0, 1.0), 0.3, 1.0, 0.3)->attenuation(1.0, 0.022, 0.0019);
        }

        void draw(Program * prog) {

            prog->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) );
            model->draw(prog);

        }
    };

    class Shadow2 : public Base {
    public:
        static int main() {

            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            UniformBuffer * cameraUBO = new UniformBuffer();
            cameraUBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();

            Shadow2Scene * scene = new Shadow2Scene();

            Program * common_program = simple_shader_program(shader_path("shadow2/object.vert"), shader_path("shadow2/object.frag"));

            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());

                printf("CamPos %.3f %.3f %.3f\r", getCamera()->position().x, getCamera()->position().y, getCamera()->position().z);

                /* Upload camera parameters */
                cameraUBO->bind()
                        ->loadSub(0, sizeof(glm::mat4), glm::value_ptr(getCamera()->projectionMatrix()))
                        ->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getCamera()->viewMatrix()))
                        ->unbind();

                common_program->use();
                common_program->setUniformBlockBinding("Camera", 0);

                scene->light->apply(common_program, getCamera()->position(), "light", "viewPos");
                scene->draw(common_program);

                GLFW::swapBuffers(window);
                GLFW::pollEvents();

            }

            return 0;
        }

    };

}


#endif //OPENGLLAB_SHADOW2_HPP
