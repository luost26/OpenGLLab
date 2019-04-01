//
//  VertexArray.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include "support.hpp"
#include <vector>

namespace wglfw {
    
    class VertexArrayNotBoundException : Exception {
        const char * what () const throw () {
            return "The given vertex array is not bound.";
        }
    };
    
    class VertexArray {
    private:
        unsigned int _VAO;
        VertexArray(unsigned int i): _VAO(i) {}
        static VertexArray * currentBinding;
        static VertexArray * previousBinding;
    public:
        VertexArray() {
            glGenVertexArrays(1, &_VAO);
        }
        
        static VertexArray * resumePreviousBinding() {
            VertexArray * prev = previousBinding;
            if (prev) {
                prev->bind();
                previousBinding = NULL;
                return prev;
            }
            return NULL;
        }
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound);
            return bound == _VAO;
        }
        
        VertexArray * bind() {
            previousBinding = currentBinding;
            
            glBindVertexArray(_VAO);
            return this;
        }
        
        VertexArray * unbind() {
            if (isBound()) {
                currentBinding = NULL;
                glBindVertexArray(0);
            }
            return this;
        }
        
        unsigned int VAO() {
            return _VAO;
        }
        
        ~VertexArray() {
            if (previousBinding == this) previousBinding = NULL;
            if (currentBinding == this) currentBinding = NULL;
            
            glDeleteVertexArrays(1, &_VAO);
        }
    };
    
    VertexArray * VertexArray::currentBinding = NULL;
    VertexArray * VertexArray::previousBinding = NULL;
    
}

#endif /* VertexArray_hpp */
