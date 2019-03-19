#include "../WrappedGLFW/loader.hpp"
#include <glm/vec4.hpp>
#include <iostream>

using namespace wglfw;

void processInput(Window *window)
{
    if (glfwGetKey(window->glfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->glfwWindow(), true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    GL::setViewport(0, 0, width, height);
}


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
    
    
    VertexShader * vShader = new VertexShader();
    vShader->source(ShaderSource::fromCString(vertexShaderSource));
    vShader->compile();
    
    FragmentShader * fShader = new FragmentShader();
    fShader->source(ShaderSource::fromCString(fragmentShaderSource));
    fShader->compile();
    
    Program * prog = new Program();
    prog->attach(vShader)
            ->attach(fShader)
            ->link();
    
    delete vShader;
    delete fShader;
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    VertexArray * VA = new VertexArray();
    VA->bind();
    
    ArrayBuffer * VB = new ArrayBuffer();
    ElementArrayBuffer * EB = new ElementArrayBuffer();

    VB->bind();
    VB->load(sizeof(vertices), vertices, GL_STATIC_DRAW);

    EB->bind();
    EB->load(sizeof(indices), indices, GL_STATIC_DRAW);

    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(0)
                                  ->size(3)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(3*sizeof(float))
                                  ->pointer((void *)0)
                                )->enable();
    
    VB->unbind();
    VA->unbind();
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!window->shouldClose())
    {
        processInput(window);
        
        GL::clear(new ColorCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)));
        
        GL::useProgram(prog);

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
