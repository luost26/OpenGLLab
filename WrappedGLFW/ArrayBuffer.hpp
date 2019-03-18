//
//  ArrayBuffer.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef ArrayBuffer_hpp
#define ArrayBuffer_hpp

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
        
        static std::vector<Buffer*> createMany(int n) {
            unsigned int * _bs = new unsigned int[n+1];
            glGenBuffers(n, _bs);
            std::vector<Buffer*> vec;
            while (n > 0) {
                vec.push_back(new Buffer(*_bs));
                ++ _bs; -- n;
            }
            delete [] _bs;
            return vec;
        }
        
        unsigned int buffer() {
            return _buffer;
        }
        
        virtual void bind();
        virtual void unbind();
        virtual bool isBound();
        virtual void load(const GLvoid * data, GLsizeiptr size, GLenum usage);
        
        ~Buffer() {
            glDeleteBuffers(1, &_buffer);
        }
    };
    
    class ArrayBuffer : public Buffer {
    public:
        bool isBound() {
            int bound;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
            return bound == _buffer;
        }
        
        void bind() {
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
        }
        
        void unbind() {
            if (isBound()) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }
        
        void load(const GLvoid * data, GLsizeiptr size, GLenum usage) {
            if (!isBound()) {
                throw BufferNotBoundException();
            }
            glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        }
    };
    
    class ElementArrayBuffer : public Buffer {
    public:
        bool isBound() {
            int bound;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound);
            return bound == _buffer;
        }
        
        void bind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
        }
        
        void unbind() {
            if (isBound()) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
        }
        
        void load(const GLvoid * data, GLsizeiptr size, GLenum usage) {
            if (!isBound()) {
                throw BufferNotBoundException();
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
        }
    };
    
}

#endif /* ArrayBuffer_hpp */
