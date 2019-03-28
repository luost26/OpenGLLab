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
            
            CleanerCollection * cleaner = new CleanerCollection;
            cleaner->add(new ColorBufferCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)))->add(new DepthBufferCleaner);
            
            while (!window->shouldClose())
            {
                stopwatch();
                
                processInput(window);
                
                GL::clear(cleaner);
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
        
    };
}

#endif /* model_hpp */
