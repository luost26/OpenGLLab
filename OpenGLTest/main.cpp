#include "../WrappedGLFW/loader.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace wglfw;
using namespace glm;

void processInput(Window *window)
{
    if (window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window->setShouldClose(true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    GL::setViewport(0, 0, width, height);
}


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vShaderPath = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/simple.vert";
const char *fShaderPath = "/Users/tommyluo/workspace/Project/OpenGLTest/Shaders/simple.frag";

int main()
{

    GLFW::initialize();

    GLFW::setWindowHints(WindowHintList::create()
                         ->contextVersionMajor(4)
                         ->contextVersionMinor(1)
                         ->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
                         ->openGLForwardCompatability(GL_TRUE)
                         );

    Window * window = new Window(new WindowConfiguration(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL"));
    GLFW::makeContextCurrent(window);
    window->setFramebufferSizeCallback(framebuffer_size_callback);

    GLFW::loadOpenGLUsingGLAD();
    
    Program * prog = new Program();
    
    try {
        prog->attach(VertexShader::make()->source(ShaderSource::fromFile(vShaderPath))->compile())
            ->attach(FragmentShader::make()->source(ShaderSource::fromFile(fShaderPath))->compile())
            ->link();
    } catch (ShaderCompilationException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (ProgramLinkException e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    VertexArray * VA = new VertexArray();
    VA->bind();
    
    ArrayBuffer * VB = new ArrayBuffer();
    VB->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);

    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(0)
                                  ->size(3)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(5*sizeof(float))
                                  ->pointer(0)
                                )->enable();
    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(2)
                                  ->size(2)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(5*sizeof(float))
                                  ->pointer((void*)(3*sizeof(float)))
                                  )->enable();
    
    Texture2D * texture = new Texture2D();
    TextureImage * timg = TextureImage::fromPath("/Users/tommyluo/workspace/Project/OpenGLTest/Textures/wall.jpg");
    texture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
            ->minFilter(GL_LINEAR)->magFilter(GL_LINEAR)
            ->loadImage(timg)->generateMipmap();
    delete timg;
    
    VB->unbind();
    VA->unbind();
    
    prog->setTexture("texture0", GL_TEXTURE0);
    
    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 trans(1.0f);
    
    CleanerCollection * cleaner = new CleanerCollection;
    cleaner->add(new ColorBufferCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)))->add(new DepthBufferCleaner);
    
    while (!window->shouldClose())
    {
        processInput(window);
        
        GL::clear(cleaner);
        
        texture->bindToTextureUnit(GL_TEXTURE0);
        
        prog->use();
        
        glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model_mat = glm::rotate(model_mat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        glm::mat4 view_mat = glm::mat4(1.0f);
        view_mat = glm::translate(view_mat, glm::vec3(0.0f, 0.0f, -3.0f));
        
        glm::mat4 proj_mat = glm::mat4(1.0f);
        proj_mat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        glm::mat4 transform = proj_mat * view_mat * model_mat;
        
//        prog->setFloat("alpha", sin(GLFW::getTime())/3.0f + 0.5f );
        prog->setFloat("alpha", 1.0);
        prog->setMatrix4("transform", transform);
        
        VA->bind();
        
        GL::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        GL::drawArrays(GL_TRIANGLES, 0, 36);
        
        GL::enableDepthTest();
        
        GLFW::swapBuffers(window);
        GLFW::pollEvents();
    }
    
    delete VA;
    delete VB;
    
    GLFW::terminate();
    return 0;
}
