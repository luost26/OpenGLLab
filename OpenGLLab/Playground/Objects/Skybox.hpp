//
//  Skybox.hpp
//
//  Created by 罗 世通 on 2019/4/5.
//

#ifndef Skybox_hpp
#define Skybox_hpp

#include <string>

#include "../../../WrappedGLFW/loader.hpp"

using namespace wglfw;

namespace playground {

    const char * SKYBOX_VERT =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
    
"out vec3 TexCoords; \n"
    
"uniform mat4 projection; \n"
"uniform mat4 view; \n"
    
"void main() { \n"
"    TexCoords = aPos; \n"
"    gl_Position = projection * view * vec4(aPos, 1.0); \n"
"}";
    
    const char * SKYBOX_FRAG =
"#version 330 core \n"
"out vec4 FragColor; \n"
    
"in vec3 TexCoords; \n"
    
"uniform samplerCube skybox; \n"
    
"void main() { \n"
"    FragColor = texture(skybox, TexCoords); \n"
"}";
    
    const float SKYBOX_VERTICES[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
    class SkyboxCubeMapIncompleteException : public Exception {
        const char * what () const throw () {
            return "SkyboxCubeMapIncompleteException";
        }
    };
    
    class SkyboxCubeMap {
    private:
        std::string _right, _left, _top, _bottom, _front, _back;
    public:
        SkyboxCubeMap() {};
        
        SkyboxCubeMap * right(std::string & path) {
            _right = path;
            return this;
        }
        
        SkyboxCubeMap * left(std::string & path) {
            _left = path;
            return this;
        }
        
        SkyboxCubeMap * top(std::string & path) {
            _top = path;
            return this;
        }
        
        SkyboxCubeMap * bottom(std::string & path) {
            _bottom = path;
            return this;
        }
        
        SkyboxCubeMap * front(std::string & path) {
            _front = path;
            return this;
        }
        
        SkyboxCubeMap * back(std::string & path) {
            _back = path;
            return this;
        }
        
        bool isComplete() {
            return !_right.empty() && !_left.empty() && !_top.empty() && !_bottom.empty() && !_front.empty() && !_back.empty();
        }
        
        void assertIsComplete() {
            if (!isComplete()) {
                throw SkyboxCubeMapIncompleteException();
            }
        }
        
        std::vector<std::string> compose() {
            std::vector<std::string> vec{
                _right, _left, _top, _bottom, _front, _back
            };
            return vec;
        }
        
    };
    
    class Skybox {
    private:
        Program * _program;
        TextureCubeMap * _cubemap;
        VertexArray * _VAO;
        ArrayBuffer * _VBO;
        
        void initShaderProgram() {
            ShaderSource * v_shader_src = ShaderSource::fromCString(SKYBOX_VERT);
            ShaderSource * f_shader_src = ShaderSource::fromCString(SKYBOX_FRAG);
            VertexShader * v_shader = VertexShader::make()->source(v_shader_src)->compile();
            FragmentShader * f_shader = FragmentShader::make()->source(f_shader_src)->compile();
            
            _program = new Program();
            _program->attach(v_shader)->attach(f_shader)->link();
            
            delete v_shader_src;
            delete f_shader_src;
            delete v_shader;
            delete f_shader;
        }
        
        void initVertexArray() {
            _VAO = new VertexArray();
            _VBO = new ArrayBuffer();
            
        }
        
    public:
        Skybox(SkyboxCubeMap * maps) {
            
        }
    };

}

#endif /* Skybox_hpp */
