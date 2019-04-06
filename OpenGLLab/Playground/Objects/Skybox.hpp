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
/* https://stackoverflow.com/questions/11685608/convention-of-faces-in-opengl-cubemapping */
"    FragColor = texture(skybox, vec3(TexCoords.x, -TexCoords.y, TexCoords.z)); \n"
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
        
        static SkyboxCubeMap * create() {
            SkyboxCubeMap * inst = new SkyboxCubeMap;
            return inst;
        }
        
        SkyboxCubeMap * right(const std::string & path) {
            _right = path;
            return this;
        }
        
        SkyboxCubeMap * left(const std::string & path) {
            _left = path;
            return this;
        }
        
        SkyboxCubeMap * top(const std::string & path) {
            _top = path;
            return this;
        }
        
        SkyboxCubeMap * bottom(const std::string & path) {
            _bottom = path;
            return this;
        }
        
        SkyboxCubeMap * front(const std::string & path) {
            _front = path;
            return this;
        }
        
        SkyboxCubeMap * back(const std::string & path) {
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
                _right, _left, _bottom, _top, _front, _back
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
            _VAO->bind();
            _VBO->bind();
            
            _VBO->load(sizeof(SKYBOX_VERTICES), SKYBOX_VERTICES, GL_STATIC_DRAW);
            _VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                            ->index(0)
                                            ->size(3)
                                            ->type(GL_FLOAT)
                                            ->normalized(GL_FALSE)
                                            ->stride(3*sizeof(float))
                                            ->pointer(0)
                                            )->enable();
            
            VertexArray::resumePreviousBinding();
        }
        
        void initTextures(SkyboxCubeMap * maps) {
            _cubemap = simple_cube_map(maps->compose());
        }
        
    public:
        Skybox(SkyboxCubeMap * maps) {
            initShaderProgram();
            initVertexArray();
            initTextures(maps);
        }
        
        void draw(Camera * camera) {
            _program->use();
            _VAO->bind();
            
            _program->setMatrix4("projection", camera->projectionMatrix());
            _program->setMatrix4("view", glm::mat4(glm::mat3(camera->viewMatrix())));
            _cubemap->bindToTextureUnit(TextureUnit::get(0));
            _program->setTexture("skybox", TextureUnit::get(0));
            
            GL::drawArrays(GL_TRIANGLES, 0, 36);
            
            VertexArray::resumePreviousBinding();
        }
    };

}

#endif /* Skybox_hpp */
