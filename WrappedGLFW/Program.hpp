//
//  Program.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Program_hpp
#define Program_hpp

#include "support.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        
        void use() {
            glUseProgram(_program);
        }
        
        bool isUsing() {
            int p;
            glGetIntegerv(GL_CURRENT_PROGRAM, &p);
            return p == _program;
        }
        
        int getUniformLocation(const char * name) {
            return glGetUniformLocation(_program, name);
        }
        
        void setFloat(const char * name, float value) {
            glUniform1f(getUniformLocation(name), value);
        }
        
        void setInt(const char * name, int value) {
            glUniform1i(getUniformLocation(name), value);
        }
        
        void setVec4(const char * name, glm::vec4 value) {
            glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
        }
        
        void setVec3(const char * name, glm::vec3 value) {
            glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
        }
        
        void setMatrix4(const char * name, glm::mat4 mat) {
            glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(mat));
        }
        
        void setTexture(const char * name, int value) {
            return setInt(name, value);
        }
        
    };
    
}

#endif /* Program_hpp */
