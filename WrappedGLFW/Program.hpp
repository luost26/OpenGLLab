//
//  Program.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Program_hpp
#define Program_hpp

#include "support.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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
        
        Program * use() {
            glUseProgram(_program);
            return this;
        }
        
        bool isUsing() {
            int p;
            glGetIntegerv(GL_CURRENT_PROGRAM, &p);
            return p == _program;
        }
        
        int getUniformLocation(const char * name) {
            return glGetUniformLocation(_program, name);
        }
        
        int getUniformBlockIndex(const char * name) {
            return glGetUniformBlockIndex(_program, name);
        }
        
        Program * setUniformBlockBinding(const char * name, unsigned int binding_point) {
            glUniformBlockBinding(_program, getUniformBlockIndex(name), binding_point);
            return this;
        }
        
        Program * setFloat(const char * name, float value) {
            glUniform1f(getUniformLocation(name), value);
            return this;
        }
        
        Program * setInt(const char * name, int value) {
            glUniform1i(getUniformLocation(name), value);
            return this;
        }
        
        Program * setVec4(const char * name, glm::vec4 value) {
            glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
            return this;
        }
        
        Program * setVec3(const char * name, glm::vec3 value) {
            glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
            return this;
        }
        
        Program * setMatrix4(const char * name, glm::mat4 mat) {
            glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(mat));
            return this;
        }

		Program * setBool(const char * name, bool value) {
			setInt(name, value ? 1 : 0);
			return this;
		}
        
        Program * setTexture(const char * name, TextureUnit * unit) {
            return setInt(name, unit->getUnitNumber());
        }
                
    };
    
}

#endif /* Program_hpp */
