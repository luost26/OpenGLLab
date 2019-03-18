//
//  Program.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Program_hpp
#define Program_hpp

#include "Shader.hpp"

namespace wglfw {
    
    class ProgramLinkException {
    private:
        int _program;
        char infoLog[512];
    public:
        ProgramLinkException(int prog): _program(prog) {}
        const char * what () throw () {
            glGetProgramInfoLog(_program, 512, NULL, infoLog);
            return infoLog;
        }
    };
    
    class Program {
    private:
        int _program;
    public:
        Program() {
            _program = glCreateProgram();
        }
        
        int program() {
            return _program;
        }
        
        Program * attach(Shader * shader) {
            glAttachShader(_program, shader->shader());
            return this;
        }
        
        void link() {
            glLinkProgram(_program);
            int success;
            glGetShaderiv(_program, GL_LINK_STATUS, &success);
            if (!success) {
                throw ProgramLinkException(_program);
            }
        }
    };
    
}

#endif /* Program_hpp */
