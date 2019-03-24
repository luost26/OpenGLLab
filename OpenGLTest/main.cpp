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

Camera * cam;
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

void processInput(Window *window)
{
    if (window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        window->setShouldClose(true);
    }
    
    if (!cam)
        return;
    float speed = 2.5 * deltaTime;
    if (window->getKey(GLFW_KEY_W) == GLFW_PRESS)
        cam->moveAhead(speed);
    if (window->getKey(GLFW_KEY_S) == GLFW_PRESS)
        cam->moveBack(speed);
    if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
        cam->moveLeft(speed);
    if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
        cam->moveRight(speed);
    
    float degspeed = 1 * deltaTime;
    if (window->getKey(GLFW_KEY_Q) == GLFW_PRESS)
        cam->lookLeft(degspeed);
    if (window->getKey(GLFW_KEY_E) == GLFW_PRESS)
        cam->lookRight(degspeed);
    if (window->getKey(GLFW_KEY_R) == GLFW_PRESS)
        cam->lookUp(degspeed);
    if (window->getKey(GLFW_KEY_F) == GLFW_PRESS)
        cam->lookDown(degspeed);
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
    
//    glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
//    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//
//    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
    
    cam = new Camera(glm::vec3(0.2, 10.0, 10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    
    while (!window->shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        GL::clear(cleaner);
        
        texture->bindToTextureUnit(GL_TEXTURE0);
        
        prog->use();
        
        glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model_mat = glm::rotate(model_mat, GLFW::getTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        
//        float radius = 10.0f;
//        float camX = sin(glfwGetTime()) * radius;
//        float camZ = cos(glfwGetTime()) * radius;
        
        glm::mat4 proj_mat = glm::mat4(1.0f);
        proj_mat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
//        prog->setFloat("alpha", sin(GLFW::getTime())/3.0f + 0.5f );
        prog->setFloat("alpha", 1.0);
        prog->setMatrix4("model", model_mat);
        prog->setMatrix4("view", *cam);
        prog->setMatrix4("proj", proj_mat);

        
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
