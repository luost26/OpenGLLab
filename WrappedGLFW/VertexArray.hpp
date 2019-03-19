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
    public:
        VertexArray() {
            glGenVertexArrays(1, &_VAO);
        }
        
        static std::vector<VertexArray*> createMany(int n) {
            unsigned int * VAOs = new unsigned int[n+1];
            glGenVertexArrays(n, VAOs);
            std::vector<VertexArray*> arrays;
            while (n > 0) {
                arrays.push_back(new VertexArray(*VAOs));
                ++ VAOs; -- n;
            }
            delete [] VAOs;
            return arrays;
        }
        
        bool isBound() {
            int bound;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound);
            return bound == _VAO;
        }
        
        VertexArray * bind() {
            glBindVertexArray(_VAO);
            return this;
        }
        
        VertexArray * unbind() {
            if (isBound()) {
                glBindVertexArray(0);
            }
            return this;
        }
        
        unsigned int VAO() {
            return _VAO;
        }
        
        ~VertexArray() {
            glDeleteVertexArrays(1, &_VAO);
        }
    };
    
}

#endif /* VertexArray_hpp */
