//
//  shadow.hpp
//
//  Created by 罗 世通 on 2019/4/15.
//

#ifndef shadow_hpp
#define shadow_hpp

#include "base.hpp"
#include "Objects/ScreenQuad.hpp"

namespace playground {
    
    class Shadow : public Base {
    public:
        inline static void drawScene(Program * floor_prog, Program * box_prog, Floor * floor, Box * box, PointLight * light,
                glm::mat4 & lightSpaceMatrix, Texture2D * shadowMap) {
            /* Draw the floor */
            floor_prog->use();
            light->apply(floor_prog, getCamera()->position(), "light", "viewPos");
            shadowMap->bindToTextureUnit(TextureUnit::get(8));
            floor_prog->setMatrix4("lightSpaceMatrix", lightSpaceMatrix)->setTexture("shadowMap", TextureUnit::get(8));
            floor->draw(floor_prog, glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10)));


            /* Draw some boxes */
            box_prog->use();
            light->apply(box_prog, getCamera()->position(), "light", "viewPos");
            box->draw(box_prog, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.8f, 2.0f)));
            box->draw(box_prog, glm::translate(glm::mat4(1.0f), glm::vec3(-3.2f, 1.0f, 1.5f)));
        }

        inline static void generateDepthMap(PointLight * light, FrameBuffer * depthMapFBO, Program * shadow_mapping_prog,
                Floor * floor, Box * box, glm::mat4 & lightSpaceMatrix) {
            /* Generate depth map */
            glm::mat4 lightProjection, lightView;
            GLfloat near_plane = 1.0f, far_plane = 7.5f;
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightView = glm::lookAt(light->getPosition(), glm::vec3(-2.0f, 0.8f, 2.0f), glm::vec3(0.0, 1.0, 0.0));
            lightSpaceMatrix = lightProjection * lightView;
            shadow_mapping_prog->use()->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);

            GL::setViewport(0, 0, 2048, 2048);
            depthMapFBO->bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            floor->draw(shadow_mapping_prog, glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10)));
            box->draw(shadow_mapping_prog, glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.8f, 2.0f)));
            box->draw(shadow_mapping_prog, glm::translate(glm::mat4(1.0f), glm::vec3(-3.2f, 1.0f, 1.5f)));

            depthMapFBO->unbind();
            GL::setViewport(0, 0, getDefaultScreenWidth()*2, getDefaultScreenHeight()*2);
        }

        static int main() {
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();

            /* Set up frame buffer for depth map */
            UniformBuffer * cameraUBO = new UniformBuffer();
            cameraUBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();

            FrameBuffer * depthMapFBO = new FrameBuffer();
            depthMapFBO->bind();

            Texture2D * depthMap = new Texture2D();
            depthMap->bind()->empty(2048, 2048, GL_DEPTH_COMPONENT, GL_FLOAT)
                    ->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)->wrapS(GL_REPEAT)->wrapT(GL_REPEAT);

            depthMapFBO->attachTexture2D(depthMap, GL_DEPTH_ATTACHMENT)->setDrawBuffer(GL_NONE)->setReadBuffer(GL_NONE)->unbind();


            /* Set up shader programs */
            Program * floor_prog = simple_shader_program(shader_path("shadow/floor.vert"), shader_path("shadow/floor.frag"));
            floor_prog->setUniformBlockBinding("Camera", 0);
            Program * box_prog = simple_shader_program(shader_path("shadow/box.vert"), shader_path("shadow/box.frag"));
            box_prog->setUniformBlockBinding("Camera", 0);

            Program * shadow_mapping_prog = simple_shader_program(shader_path("shadow/shadow_mapping_depth.vert"), shader_path("shadow/shadow_mapping_depth.frag"));

            /* Set up objects in the scene */
            Floor * floor = new Floor(10.0f);floor->colorTexture(simple_texture(texture_path("wood.png")));
            Box * box = new Box(simple_texture(texture_path("container2.png")), simple_texture(texture_path("container2_specular.png")));
            glm::vec3 light_pos = glm::vec3(0.0, 5.0, 0.0);
            PointLight * light = PointLight::create()->position(light_pos)
                    ->color(glm::vec3(1.0, 1.0, 1.0), 0.05, 1.0, 0.3)->attenuation(1.0, 0.022, 0.0019);


            /* Set the initial view */
            getCamera()->moveUp(1.0f);
            ScreenQuad * quad = new ScreenQuad(NULL, load_fragment_shader(shader_path("shadow/debug_quad.frag")));
//            ScreenQuad * quad = new ScreenQuad;

            Texture2D * test_tex = simple_texture(texture_path("wood.png"));

            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());

                /* Upload camera parameters */
                cameraUBO->bind()
                    ->loadSub(0, sizeof(glm::mat4), glm::value_ptr(getCamera()->projectionMatrix()))
                    ->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getCamera()->viewMatrix()))
                    ->unbind();

                glm::mat4 lightSpaceMatrix = glm::mat4(1.0);
                generateDepthMap(light, depthMapFBO, shadow_mapping_prog, floor, box, lightSpaceMatrix);

                drawScene(floor_prog, box_prog, floor, box, light, lightSpaceMatrix, depthMap);

                /* Show depth map */
//                GL::clear(getDefaultCleanerCollection());
//                quad->draw(depthMap);

                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
}

#endif /* shadow_hpp */