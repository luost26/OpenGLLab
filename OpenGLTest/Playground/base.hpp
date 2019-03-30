//
//  base.hpp
//
//  Created by 罗 世通 on 2019/3/24.
//

#ifndef base_hpp
#define base_hpp

#include "../../WrappedGLFW/loader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace wglfw;

namespace playground {
    class Base {
    public:
        
        static Camera * cam;
        static float deltaTime;
        static float lastFrame;
        
        static int main() {
            return -1;
        };
        
        static Camera * getCamera() {
            if (cam == NULL) {
                cam = new Camera(glm::vec3(0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            }
            return cam;
        }
        
        static void stopwatch() {
            float currentFrame = GLFW::getTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }
        
        static unsigned int getDefaultScreenWidth() {
            return 800;
        }
        
        static unsigned int getDefaultScreenHeight() {
            return 600;
        }
        
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
        
        static Window * initializeEnvAndCreateWindow() {
            GLFW::initialize();
            
            GLFW::setWindowHints(WindowHintList::create()
                                 ->contextVersionMajor(4)
                                 ->contextVersionMinor(1)
                                 ->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
                                 ->openGLForwardCompatability(GL_TRUE)
                                 );
            
            Window * window = new Window(new WindowConfiguration(getDefaultScreenWidth(), getDefaultScreenHeight(), "Playground"));
            GLFW::makeContextCurrent(window);
            window->setFramebufferSizeCallback(framebuffer_size_callback);
            
            GLFW::loadOpenGLUsingGLAD();
            
            return window;
        }
        
        static inline void setProjectionViewMatrixOfProgram(Program * prog, const std::string & proj_name = "projection", const std::string & view_name = "view") {
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)getDefaultScreenWidth() / (float)getDefaultScreenHeight(), 0.1f, 100.0f);
            glm::mat4 view = getCamera()->viewMatrix();
            prog->setMatrix4(proj_name.c_str(), projection);
            prog->setMatrix4(view_name.c_str(), view);
        }
        
    };
    
    Camera * Base::cam = NULL;
    float Base::deltaTime = 0;
    float Base::lastFrame = 0;
}

#endif /* base_hpp */
