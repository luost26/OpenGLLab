//
//  Shader.hpp
//
//  Created by 罗 世通 on 2019/3/18.
//

#ifndef Shader_hpp
#define Shader_hpp

#include "support.hpp"
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

namespace wglfw {
    
    class ShaderCompilationException {
    private:
        int _shader;
        char infoLog[512];
    public:
        ShaderCompilationException(int s): _shader(s) {}
        const char * what () throw () {
            glGetShaderInfoLog(_shader, 512, NULL, infoLog);
            return infoLog;
        }
    };
    
    class ShaderSource {
    private:
        char * src;
        ShaderSource() {}
    public:
        static ShaderSource * fromCString(const char * str) {
            ShaderSource * instance = new ShaderSource();
            instance->src = new char[strlen(str) + 1];
            strcpy(instance->src, str);
            return instance;
        }
        
        static ShaderSource * fromString(std::string str) {
            return fromCString(str.c_str());
        }
        
        static ShaderSource * fromFile(const char * path) {
            std::string srcstr;
            std::ifstream file;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            file.open(path);
            std::stringstream ss;
            ss << file.rdbuf();
            file.close();
            srcstr = ss.str();
            return fromString(srcstr);
        }
        
        char * source() {
            return src;
        }
        
        ~ShaderSource() {
            delete [] src;
        }
    };
    
    class Shader {
    protected:
        int _shader;
    public:
        Shader(int type) {
            _shader = glCreateShader(type);
        }
        
        int shader() {
            return _shader;
        }
        
        Shader * source(ShaderSource * source) {
            char * src = source->source();
            glShaderSource(_shader, 1, &src, NULL);
            return this;
        }
        
        Shader * compile() {
            glCompileShader(_shader);
            int success;
            glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                throw ShaderCompilationException(_shader);
            }
            return this;
        }
        
        ~Shader() {
            glDeleteShader(_shader);
        }
    };
    
    class VertexShader : public Shader {
    public:
        static VertexShader * make() {
            return new VertexShader();
        }
        
        VertexShader(): Shader(GL_VERTEX_SHADER) {}
    };
    
    class FragmentShader : public Shader {
    public:
        static FragmentShader * make() {
            return new FragmentShader();
        }
        
        FragmentShader(): Shader(GL_FRAGMENT_SHADER) {}
    };
}

#endif /* Shader_hpp */
