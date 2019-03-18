//
//  GL.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef GL_hpp
#define GL_hpp

#include "support.hpp"
#include "Program.hpp"
#include <glm/vec4.hpp>

namespace wglfw {
    
    class Cleaner {
    public:
        virtual void clean();
    };
    
    class ColorCleaner : public Cleaner {
    public:
        glm::vec4 rgba;
        ColorCleaner(glm::vec4 color):rgba(color) {};
        void clean() {
            glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    };
    
    
    class GL {
    public:
        static void useProgram(Program * prog) {
            glUseProgram(prog->program());
        }
        
        static void clear(Cleaner * cleaner) {
            cleaner->clean();
        }
        
    };
    
}

#endif /* GL_hpp */