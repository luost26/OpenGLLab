//
//  GL.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef GL_hpp
#define GL_hpp

#include "support.hpp"
#include "Program.hpp"

namespace wglfw {
    
    class Cleaner {
    public:
        virtual void prepare() {}
        virtual unsigned int option() {
            return 0;
        }
    };
    
    class ColorBufferCleaner : public Cleaner {
    public:
        glm::vec4 rgba;
        ColorBufferCleaner(glm::vec4 color):rgba(color) {};
        void prepare() {
            glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
        }
        
        unsigned int option() {
            return GL_COLOR_BUFFER_BIT;
        }
    };
    
    class DepthBufferCleaner : public Cleaner {
    public:
        unsigned int option() {
            return GL_DEPTH_BUFFER_BIT;
        }
    };
    
    class StencilBufferCleaner : public Cleaner {
    public:
        unsigned int option() {
            return GL_STENCIL_BUFFER_BIT;
        }
    };
    
    
    class CleanerCollection {
    private:
        std::vector<Cleaner *> collection;
    public:
        CleanerCollection * add(Cleaner * cleaner) {
            collection.push_back(cleaner);
            return this;
        }

		~CleanerCollection() {
			for (auto x : collection) {
				delete x;
			}
		}
        
        void prepare() {
            for (auto x : collection) {
                x->prepare();
            }
        }
        
        unsigned int option() {
            unsigned int merged = 0;
            for (auto x : collection) {
                merged |= x->option();
            }
            return merged;
        }
    };
    
    class GL {
    public:
        static void useProgram(Program * prog) {
            glUseProgram(prog->program());
        }
        
        static void clear(CleanerCollection * c) {
            c->prepare();
            glClear(c->option());
        }
        
        static void setViewport(GLint x, GLint y, GLsizei width, GLsizei height, GLuint scale=1) {
            glViewport(x, y, width*scale, height*scale);
        }
        
        static void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices) {
            glDrawElements(mode, count, type, indices);
        }
        
        static void drawArrays(GLenum mode, GLint first, GLsizei count) {
            glDrawArrays(mode, first, count);
        }
        
        static void setPolygonMode(GLenum face, GLenum mode) {
            glPolygonMode(face, mode);
        }
        
        static void enable(GLenum cap) {
            glEnable(cap);
        }

		static void disable(GLenum cap) {
			glDisable(cap);
		}
        
        static void enableDepthTest() {
            glEnable(GL_DEPTH_TEST);
        }
    };
    
}

#endif /* GL_hpp */
