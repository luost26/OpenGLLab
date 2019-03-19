#include "../WrappedGLFW/loader.hpp"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

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
    
    struct ColoredVertex {
        vec3 pos, rgb;
        ColoredVertex(vec3 p, vec3 c): pos(p), rgb(c) {};
        ColoredVertex() {};
    };
    
    ColoredVertex vertices[] = {
        ColoredVertex(vec3(0.5, 0.5, 0), vec3(1.0, 0, 0)),
        ColoredVertex(vec3(0.5, -0.5, 0), vec3(0, 1.0, 0)),
        ColoredVertex(vec3(-0.5, -0.5, 0), vec3(0, 0, 1.0)),
        ColoredVertex(vec3(-0.5, 0.5, 0), vec3(1.0, 0, 1.0))
    };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    VertexArray * VA = new VertexArray();
    VA->bind();
    
    ArrayBuffer * VB = new ArrayBuffer();
    ElementArrayBuffer * EB = new ElementArrayBuffer();

    VB->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);

    EB->bind()->load(sizeof(indices), indices, GL_STATIC_DRAW);

    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(0)
                                  ->size(3)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(sizeof(ColoredVertex))
                                  ->pointer(0)
                                )->enable();
    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(1)
                                  ->size(3)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(sizeof(ColoredVertex))
                                  ->pointer((void*)sizeof(glm::vec3))
                                  )->enable();
    
    VB->unbind();
    VA->unbind();
    
    // uncomment this call to draw in wireframe polygons.
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window->shouldClose())
    {
        processInput(window);
        
        GL::clear(new ColorCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)));
        
        GL::useProgram(prog);
        
//        prog->setVec4("ourColor", glm::vec4(sin((double)rand()), cos((double)rand()), sin((double)rand()), sin((double)rand())) );

        VA->bind();
        
        GL::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        GLFW::swapBuffers(window);
        GLFW::pollEvents();
    }
    
    delete VA;
    delete VB;
    delete EB;
    
    GLFW::terminate();
    return 0;
}
