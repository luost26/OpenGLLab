//
//  basic.hpp
//
//  Created by 罗 世通 on 2019/3/24.
//

#ifndef basic_hpp
#define basic_hpp

#include "base.hpp"

using namespace wglfw;
using namespace glm;

namespace playground {
    class Basic : public Base {
    public:
        static Camera * cam;
        static float deltaTime;
        static float lastFrame;
        
        const static unsigned int SCR_WIDTH;
        const static unsigned int SCR_HEIGHT;
        
        const static char *vShaderPath;
        const static char *fShaderPath;
        
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
            if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
                cam->moveLeft(speed);
            if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
                cam->moveRight(speed);
            
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
            
            Window * window = new Window(new WindowConfiguration(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL"));
            GLFW::makeContextCurrent(window);
            window->setFramebufferSizeCallback(framebuffer_size_callback);
            
            GLFW::loadOpenGLUsingGLAD();
            
            Program * prog = new Program();
            
            try {
                prog->attach(VertexShader::make()->source(ShaderSource::fromFile(vShaderPath))->compile())
                ->attach(FragmentShader::make()->source(ShaderSource::fromFile(fShaderPath))->compile())
                ->link();
            } catch (ShaderCompilationException e) {
                std::cerr << e.what() << std::endl;
                return -1;
            } catch (ProgramLinkException e) {
                std::cerr << e.what() << std::endl;
                return -1;
            }
            
            float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };
            
            VertexArray * VA = new VertexArray();
            VA->bind();
            
            ArrayBuffer * VB = new ArrayBuffer();
            VB->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
            
            VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                          ->index(0)
                                          ->size(3)
                                          ->type(GL_FLOAT)
                                          ->normalized(GL_FALSE)
                                          ->stride(5*sizeof(float))
                                          ->pointer(0)
                                          )->enable();
            VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                          ->index(2)
                                          ->size(2)
                                          ->type(GL_FLOAT)
                                          ->normalized(GL_FALSE)
                                          ->stride(5*sizeof(float))
                                          ->pointer((void*)(3*sizeof(float)))
                                          )->enable();
            
            Texture2D * texture = new Texture2D();
            TextureImage * timg = TextureImage::fromPath("/Users/tommyluo/workspace/Project/OpenGLTest/Textures/wall.jpg");
            texture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
            ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR)
            ->loadImage(timg)->generateMipmap();
            delete timg;
            
            VB->unbind();
            VA->unbind();
            
            prog->setTexture("texture0", GL_TEXTURE0);
            
            glm::mat4 trans(1.0f);
            
            CleanerCollection * cleaner = new CleanerCollection;
            cleaner->add(new ColorBufferCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)))->add(new DepthBufferCleaner);
            
            cam = new Camera(glm::vec3(0.2, 10.0, 10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            
            while (!window->shouldClose())
            {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                
                processInput(window);
                
                GL::clear(cleaner);
                
                texture->bindToTextureUnit(GL_TEXTURE0);
                
                prog->use();
                
                glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model_mat = glm::rotate(model_mat, GLFW::getTime(), glm::vec3(1.0f, 1.0f, 0.0f));
                
                //        float radius = 10.0f;
                //        float camX = sin(glfwGetTime()) * radius;
                //        float camZ = cos(glfwGetTime()) * radius;
                
                glm::mat4 proj_mat = glm::mat4(1.0f);
                proj_mat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                
                
                float value = sin(GLFW::getTime())/2.0f + 0.5f;
                prog->setFloat("alpha", 1.0);
                prog->setVec4("lightColor", glm::vec4(value, 1.0, value, 1.0));
                prog->setMatrix4("model", model_mat);
                prog->setMatrix4("view", *cam);
                prog->setMatrix4("proj", proj_mat);
                
                
                VA->bind();
                
                GL::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                GL::drawArrays(GL_TRIANGLES, 0, 36);
                
                GL::enableDepthTest();
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            delete VA;
            delete VB;
            
            GLFW::terminate();
            return 0;
        }
    };
    
    Camera * Basic::cam = NULL;
    float Basic::deltaTime = 0.0f;    // time between current frame and last frame
    float Basic::lastFrame = 0.0f;
    
    const unsigned int Basic::SCR_WIDTH = 800;
    const unsigned int Basic::SCR_HEIGHT = 600;
    
    const char *Basic::vShaderPath = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic/simple.vert";
    const char *Basic::fShaderPath = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/basic/simple.frag";
}

#endif /* basic_hpp */
