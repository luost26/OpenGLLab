//
//  basic_lighting.hpp
//
//  Created by 罗 世通 on 2019/3/24.
//

#ifndef basic_lighting_hpp
#define basic_lighting_hpp

#include "base.hpp"

using namespace wglfw;
using namespace glm;

namespace playground {
    class BasicLighting : public Base {
    public:
        static Camera * cam;
        static float deltaTime;
        static float lastFrame;
        
        const static unsigned int SCR_WIDTH;
        const static unsigned int SCR_HEIGHT;
        
        static void processInput(Window *window) {
            if (window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window->setShouldClose(true);
            }
            
            if (!cam)
                return;
            float speed = 2.5 * deltaTime;
            if (window->getKey(GLFW_KEY_W) == GLFW_PRESS)
                cam->moveAhead(speed);
            if (window->getKey(GLFW_KEY_S) == GLFW_PRESS)
                cam->moveBack(speed);
            if (window->getKey(GLFW_KEY_A) == GLFW_PRESS || window->getKey(GLFW_KEY_LEFT) == GLFW_PRESS)
                cam->moveLeft(speed);
            if (window->getKey(GLFW_KEY_D) == GLFW_PRESS || window->getKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
                cam->moveRight(speed);
            if (window->getKey(GLFW_KEY_UP) == GLFW_PRESS)
                cam->moveUp(speed);
            if (window->getKey(GLFW_KEY_DOWN) == GLFW_PRESS)
                cam->moveDown(speed);
            
            float degspeed = 1 * deltaTime;
            if (window->getKey(GLFW_KEY_Q) == GLFW_PRESS)
                cam->lookLeft(degspeed);
            if (window->getKey(GLFW_KEY_E) == GLFW_PRESS)
                cam->lookRight(degspeed);
            if (window->getKey(GLFW_KEY_R) == GLFW_PRESS)
                cam->lookUp(degspeed);
            if (window->getKey(GLFW_KEY_F) == GLFW_PRESS)
                cam->lookDown(degspeed);
        }
        
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            GL::setViewport(0, 0, width, height);
        }
        
        static int main() {
            GLFW::initialize();
            
            GLFW::setWindowHints(WindowHintList::create()
                                 ->contextVersionMajor(4)
                                 ->contextVersionMinor(1)
                                 ->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
                                 ->openGLForwardCompatability(GL_TRUE)
                                 );
            
            Window * window = new Window(new WindowConfiguration(SCR_WIDTH, SCR_HEIGHT, "Lighting"));
            GLFW::makeContextCurrent(window);
            window->setFramebufferSizeCallback(framebuffer_size_callback);
            
            GLFW::loadOpenGLUsingGLAD();
            
            GL::enableDepthTest();
            
            Program * lampProgram = new Program();
            Program * objectProgram = new Program();
            
            try {
                lampProgram->attach(VertexShader::make()
                                ->source(ShaderSource::fromFile("/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic_lighting/basic.vert"))
                                ->compile())
                    ->attach(FragmentShader::make()
                                ->source(ShaderSource::fromFile("/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic_lighting/lamp.frag"))
                                ->compile())
                    ->link();
                
                objectProgram->attach(VertexShader::make()
                                      ->source(ShaderSource::fromFile("/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic_lighting/basic.vert"))
                                      ->compile())
                    ->attach(FragmentShader::make()
                                ->source(ShaderSource::fromFile("/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic_lighting/object.frag"))
                                ->compile())
                    ->link();
            } catch (ShaderCompilationException e) {
                std::cerr << e.what() << std::endl;
                return -1;
            } catch (ProgramLinkException e) {
                std::cerr << e.what() << std::endl;
                return -1;
            }
            
            float vertices[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
            };
            
            VertexArray * cubeVAO = new VertexArray();
            cubeVAO->bind();
            
            ArrayBuffer * VBO = new ArrayBuffer();
            VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                          ->index(0)
                                          ->size(3)
                                          ->type(GL_FLOAT)
                                          ->normalized(GL_FALSE)
                                          ->stride(8*sizeof(float))
                                          ->pointer(0)
                                          )->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(1)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer((void *)(3*sizeof(float)))
                                           )->enable();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(2)
                                           ->size(2)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer((void *)(6*sizeof(float)))
                                           )->enable();
            
            VertexArray * lampVAO = new VertexArray();
            lampVAO->bind();
            VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                           ->index(0)
                                           ->size(3)
                                           ->type(GL_FLOAT)
                                           ->normalized(GL_FALSE)
                                           ->stride(8*sizeof(float))
                                           ->pointer(0)
                                           )->enable();
            
            CleanerCollection * cleaners = new CleanerCollection();
            cleaners->add(new ColorBufferCleaner(vec4(0.1f, 0.1f, 0.1f, 1.0f)))
                    ->add(new DepthBufferCleaner());
            
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            
            cam = new Camera(glm::vec3(0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            
            glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
            glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
            
            glm::vec3 lampPos(1.2f, 0.0f, 0.0f);
            glm::mat4 lampModel = glm::mat4(1.0f);
            lampModel = glm::translate(lampModel, lampPos);
            lampModel = glm::scale(lampModel, glm::vec3(0.2f));
            

            Texture2D * texture = new Texture2D();
            TextureImage * timg = TextureImage::fromPath("/Users/tommyluo/workspace/Project/OpenGLTest/Textures/container2.png");
            texture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
                    ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR)
                    ->loadImage(timg)->generateMipmap();
            delete timg;
            
            Texture2D * specTexture = new Texture2D();
            TextureImage * spectimg = TextureImage::fromPath("/Users/tommyluo/workspace/Project/OpenGLTest/Textures/lighting_maps_specular_color.png");
            specTexture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
                ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR)
                ->loadImage(spectimg)->generateMipmap();
            delete spectimg;
            
            Texture2D * emisTexture = new Texture2D();
            TextureImage * emistimg = TextureImage::fromPath("/Users/tommyluo/workspace/Project/OpenGLTest/Textures/matrix.jpg");
            emisTexture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
                ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR)
                ->loadImage(emistimg)->generateMipmap();
            delete emistimg;
            
            
            
            while (!window->shouldClose()) {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                
                processInput(window);
                
                GL::clear(cleaners);
                
//                lightColor.x = sin(glfwGetTime() * 2.0f);
//                lightColor.y = sin(glfwGetTime() * 0.7f);
//                lightColor.z = sin(glfwGetTime() * 1.3f);
                
                texture->bindToTextureUnit(TextureUnit::get(0));
                specTexture->bindToTextureUnit(TextureUnit::get(1));
                emisTexture->bindToTextureUnit(TextureUnit::get(2));
                
                
                objectProgram->use()
                            ->setVec3("light.ambient", glm::vec3(0.1) * lightColor)
                            ->setVec3("light.diffuse", glm::vec3(0.8) * lightColor)
                            ->setVec3("light.specular", glm::vec3(1.0) * lightColor)
                            ->setVec3("light.position", lampPos)
                            ->setFloat("material.shininess", 32)
                            ->setTexture("material.diffuse", TextureUnit::get(0))
                            ->setTexture("material.specular", TextureUnit::get(1))
                            ->setTexture("material.emission", TextureUnit::get(2))
                            ->setMatrix4("projection", projection)
                            ->setMatrix4("view", *cam)
                            ->setMatrix4("model", glm::mat4(1.0f))
                            ->setVec3("viewPos", cam->position());
                cubeVAO->bind();
                GL::drawArrays(GL_TRIANGLES, 0, 36);
                
                lampProgram->use()
                            ->setVec3("lightColor", lightColor)
                            ->setMatrix4("projection", projection)
                            ->setMatrix4("view", *cam)
                            ->setMatrix4("model", lampModel);
                lampVAO->bind();
                GL::drawArrays(GL_TRIANGLES, 0, 36);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
    Camera * BasicLighting::cam = NULL;
    float BasicLighting::deltaTime = 0.0f;    // time between current frame and last frame
    float BasicLighting::lastFrame = 0.0f;
    
    const unsigned int BasicLighting::SCR_WIDTH = 800;
    const unsigned int BasicLighting::SCR_HEIGHT = 600;
}



#endif /* basic_lighting_hpp */
