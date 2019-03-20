//
//  GL.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef GL_hpp
#define GL_hpp

#include "support.hpp"
#include "Program.hpp"
#include <glm/glm.hpp>

namespace wglfw {
    
    class Cleaner {
    public:
        virtual void clean() {}
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
        
        static void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
            glViewport(x, y, width, height);
        }
        
        static void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices) {
            glDrawElements(mode, count, type, indices);
        }
        
        static void setPolygonMode(GLenum face, GLenum mode) {
            glPolygonMode(face, mode);
        }
    };
    
}

#endif /* GL_hpp */
