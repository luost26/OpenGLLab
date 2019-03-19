//
//  GLFW.hpp
//
//  Created by 罗 世通 on 2019/3/18.
//

#ifndef GLFW_hpp
#define GLFW_hpp

#include "support.hpp"
#include "Window.hpp"
#include <map>

namespace wglfw {
    
    class WindowHintList {
    public:
        std::map<int, int> hints;
        
        static WindowHintList * create() {
            return new WindowHintList();
        }
        
        WindowHintList * set(int hint, int value) {
            hints[hint] = value;
            return this;
        }
        
        WindowHintList * contextVersionMajor(int value) {
            hints[GLFW_CONTEXT_VERSION_MAJOR] = value;
            return this;
        }
        
        WindowHintList * contextVersionMinor(int value) {
            hints[GLFW_CONTEXT_VERSION_MINOR] = value;
            return this;
        }
        
        WindowHintList * openGLProfile(int value) {
            hints[GLFW_OPENGL_PROFILE] = value;
            return this;
        }
        
        WindowHintList * openGLForwardCompatability(int value) {
            hints[GLFW_OPENGL_FORWARD_COMPAT] = value;
            return this;
        }
        
    };
    
    class GLFW {
    private:
        static GLFW * _instance;
        GLFW() {};
        
    public:
        static GLFW * initialize() {
            glfwInit();
            if (_instance == NULL)
                _instance = new GLFW();
            return _instance;
        }
        
        static void terminate() {
            glfwTerminate();
            delete _instance;
            _instance = NULL;
        }
        
        static bool isInitialized() {
            return _instance != NULL;
        }
        
        static void setWindowHints(WindowHintList * hints) {
            for (auto pair : hints->hints) {
                glfwWindowHint(pair.first, pair.second);
            }
        }
        
        static void makeContextCurrent(Window * window) {
            glfwMakeContextCurrent(window->glfwWindow());
        }
      
        static void swapBuffers(Window * window) {
            glfwSwapBuffers(window->glfwWindow());
        }
        
        static void pollEvents() {
            glfwPollEvents();
        }
        
        static void loadOpenGLUsingGLAD() {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw Exception();
            }
        }
        
        static float getTime() {
            return glfwGetTime();
        }
        
    };
    
    GLFW * GLFW::_instance = NULL;
}

#endif /* GLFW_hpp */
