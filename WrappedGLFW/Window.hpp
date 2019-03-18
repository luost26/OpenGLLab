//
//  Window.hpp
//  OpenGLTest
//
//  Created by 罗 世通 on 2019/3/18.
//  Copyright © 2019 罗 世通. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include "support.hpp"
#include "Exception.hpp"
#include <string>

namespace wglfw {
    
    class WindowConfiguration {
    public:
        int width, height;
        std::string title;
        WindowConfiguration(int w, int h, const std::string t): width(w), height(h), title(t) {}
    };
    
    class WindowCreationException : Exception {
        const char * what () const throw ()
        {
            return "Failed to create GLFW window";
        }
    };
    
    class Window {
    private:
        GLFWwindow * _window;
        
    public:
        Window(WindowConfiguration * config) {
            _window = glfwCreateWindow(config->width, config->height, config->title.c_str(), NULL, NULL);
            if (_window == NULL) {
                throw WindowCreationException();
            }
        }
        
        GLFWwindow * glfwWindow() {
            return _window;
        }
        
        void setFramebufferSizeCallback(GLFWframebuffersizefun cbfun) {
            glfwSetFramebufferSizeCallback(_window, cbfun);
        }
        
    };
    
    
    
}

#endif /* Window_hpp */
