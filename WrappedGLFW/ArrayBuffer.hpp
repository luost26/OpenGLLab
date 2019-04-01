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
        
        unsigned int buffer() {
            return _buffer;
        }
        
        virtual Buffer * bind() {
            return this;
        }
        virtual Buffer * unbind() {
            return this;
        }
        virtual bool isBound() {
            return false;
        }
        virtual Buffer * load(GLsizeiptr size, const GLvoid * data, GLenum usage) {
            return this;
        }
        
        virtual ~Buffer() {
            glDeleteBuffers(1, &_buffer);
        }
    };
    
    class VertexAttributePointer {        
    public:
        unsigned int index;
        VertexAttributePointer(unsigned int idx): index(idx) {}
        void enable() {
            glEnableVertexAttribArray(index);
        }
    };
    
    class VertexAttributePointerConfiguration {
    private:
        VertexAttributePointerConfiguration() {}
    public:
        GLuint _index;
        GLint _size;
        GLenum _type;
        GLboolean _normalized;
        GLsizei _stride;
        GLvoid * _pointer;
        
        VertexAttributePointerConfiguration(GLuint _i, GLint _s, GLenum _t, GLboolean _n, GLsizei _st, GLvoid * _p): _index(_i), _size(_s), _type(_t), _normalized(_n), _stride(_st), _pointer(_p) {}
        
        static VertexAttributePointerConfiguration * make() {
            return new VertexAttributePointerConfiguration();
        }
        
        VertexAttributePointerConfiguration * index(GLuint i) {
            _index = i;
            return this;
        }
        
        VertexAttributePointerConfiguration * size(GLint s) {
            _size = s;
            return this;
        }
        
        VertexAttributePointerConfiguration * type(GLenum t) {
            _type = t;
            return this;
        }
        
        VertexAttributePointerConfiguration * normalized(GLboolean n) {
            _normalized = n;
            return this;
        }
        
        VertexAttributePointerConfiguration * stride(GLsizei s) {
            _stride = s;
            return this;
        }
        
        VertexAttributePointerConfiguration * pointer(GLvoid * p) {
            _pointer = p;
            return this;
        }
        
    };
    
    class ArrayBuffer : public Buffer {
    public:
        ArrayBuffer() : Buffer() {}
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
            return bound == _buffer;
        }
        
        ArrayBuffer * bind() {
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            return this;
        }
        
        ArrayBuffer * unbind() {
            if (isBound()) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            return this;
        }
        
        ArrayBuffer * load(GLsizeiptr size, const GLvoid * data, GLenum usage) {
            if (!isBound()) {
                throw BufferNotBoundException();
            }
            glBufferData(GL_ARRAY_BUFFER, size, data, usage);
            return this;
        }
        
        VertexAttributePointer * getVertexAttributePointer(VertexAttributePointerConfiguration * config) {
            if (!isBound()) {
                throw BufferNotBoundException();
            }
            glVertexAttribPointer(config->_index, config->_size, config->_type, config->_normalized, config->_stride, config->_pointer);
            return new VertexAttributePointer(config->_index);
        }
        
    };
    
    class ElementArrayBuffer : public Buffer {
    public:
        ElementArrayBuffer() : Buffer() {}
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound);
            return bound == _buffer;
        }
        
        ElementArrayBuffer * bind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
            return this;
        }
        
        ElementArrayBuffer * unbind() {
            if (isBound()) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
            return this;
        }
        
        ElementArrayBuffer * load(GLsizeiptr size, const GLvoid * data, GLenum usage) {
            if (!isBound()) {
                throw BufferNotBoundException();
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
            return this;
        }
    };
    
}

#endif /* ArrayBuffer_hpp */
