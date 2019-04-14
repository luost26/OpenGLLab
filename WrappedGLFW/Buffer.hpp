//
//  Buffer.hpp
//
//  Created by 罗 世通 on 2019/4/11.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include "support.hpp"

namespace wglfw {
    
    class BufferNotBoundException : Exception {
        const char * what () const throw () {
            return "The given buffer is not bound.";
        }
    };
    
    class Buffer {
    protected:
        unsigned int _buffer;
        Buffer(unsigned int i): _buffer(i) {}
    public:
        Buffer() {
            glGenBuffers(1, &_buffer);
        }
        
        unsigned int buffer() {
            return _buffer;
        }
        
        virtual Buffer * bind() {
            throw MethodNotImplementedException();
            return this;
        }
        virtual Buffer * unbind() {
            throw MethodNotImplementedException();
            return this;
        }
        virtual bool isBound() {
            throw MethodNotImplementedException();
            return false;
        }
        virtual Buffer * load(GLsizeiptr size, const GLvoid * data, GLenum usage) {
            throw MethodNotImplementedException();
            return this;
        }
        virtual Buffer * loadSub(GLintptr offset, GLsizeiptr size, const GLvoid * data) {
            throw MethodNotImplementedException();
            return this;
        }
        virtual ~Buffer() {
            glDeleteBuffers(1, &_buffer);
        }
    };
    
}

#endif /* Buffer_hpp */
