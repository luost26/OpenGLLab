//
//  shadow.hpp
//
//  Created by 罗 世通 on 2019/4/15.
//

#ifndef shadow_hpp
#define shadow_hpp

#include "base.hpp"

namespace playground {
    
    class Shadow : public Base {
    public:
        static int main() {
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
}

#endif /* shadow_hpp */
