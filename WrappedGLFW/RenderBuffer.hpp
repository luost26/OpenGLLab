//
//  RenderBuffer.hpp
//
//  Created by 罗 世通 on 2019/4/1.
//

#ifndef RenderBuffer_hpp
#define RenderBuffer_hpp

#include "support.hpp"

namespace wglfw {
    
    class RenderBufferNotBoundException : public Exception {
        const char * what () const throw () {
            return "The given render buffer is not bound.";
        }
    };
    
    class RenderBuffer {
    private:
        unsigned int _RBO;
    public:
        RenderBuffer() {
            glGenRenderbuffers(1, &_RBO);
        }
        
        RenderBuffer * bind() {
            glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
            return this;
        }
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_RENDERBUFFER_BINDING, &bound);
            return bound == _RBO;
        }
        
        void assertIsBound() {
            if (!isBound()) {
                throw RenderBufferNotBoundException();
            }
        }
        
        RenderBuffer * unbind() {
            assertIsBound();
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            return this;
        }
        
        RenderBuffer * storage(unsigned int width, unsigned int height, GLenum format=GL_DEPTH24_STENCIL8) {
            assertIsBound();
            glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
            return this;
        }
        
        unsigned int RBO() {
            return _RBO;
        }
    };
    
}

#endif /* RenderBuffer_hpp */
