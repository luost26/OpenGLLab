//
//  UniformBuffer.hpp
//
//  Created by 罗 世通 on 2019/4/11.
//

#ifndef UniformBuffer_hpp
#define UniformBuffer_hpp

#include "support.hpp"
#include "Buffer.hpp"

namespace wglfw {
    
    class UniformBuffer : public Buffer{
    private:
        GLsizeiptr _size;
    public:
        UniformBuffer() : Buffer() {}
        
        UniformBuffer * bind() {
            glBindBuffer(GL_UNIFORM_BUFFER, _buffer);
            return this;
        }
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &bound);
            return bound == _buffer;
        }
        
        UniformBuffer * unbind() {
            if (isBound()) {
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
            }
            return this;
        }
        
        UniformBuffer * load(GLsizeiptr size, const GLvoid * data, GLenum usage) {
            glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
            _size = size;
            return this;
        }
        
        UniformBuffer * loadSub(GLintptr offset, GLsizeiptr size, const GLvoid * data) {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
            return this;
        }
        
        UniformBuffer * bindRange(GLuint binding_point) {
            glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, _buffer, 0, _size);
            return this;
        }
        
        UniformBuffer * bindRange(GLuint binding_point, GLintptr offset, GLsizeiptr size) {
            glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, _buffer, offset, size);
            return this;
        }
        
    };
    
}

#endif /* UniformBuffer_hpp */
