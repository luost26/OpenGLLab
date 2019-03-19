#include "../WrappedGLFW/loader.hpp"
#include <glm/vec4.hpp>
#include <iostream>

using namespace wglfw;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(Window *window)
{
    if (glfwGetKey(window->glfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->glfwWindow(), true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
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
#ifdef __APPLE__
    GLFW::setWindowHints(WindowHintList::create()
                         ->contextVersionMajor(4)
                         ->contextVersionMinor(1)
                         ->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
                         ->openGLForwardCompatability(GL_TRUE)
                         );
#else
    GLFW::setWindowHints(WindowHintList::create()
                         ->contextVersionMajor(4)
                         ->contextVersionMinor(1)
                         ->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
                         );
#endif
    
    // glfw window creation
    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    Window * window = new Window(new WindowConfiguration(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL"));
    GLFW::makeContextCurrent(window);
    window->setFramebufferSizeCallback(framebuffer_size_callback);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
    GLFW::loadOpenGLUsingGLAD();
    
    
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
//    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//    // check for shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
    
    VertexShader * vShader = new VertexShader();
    vShader->source(ShaderSource::fromCString(vertexShaderSource));
    vShader->compile();
    
//    // fragment shader
//    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//    // check for shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
    
    FragmentShader * fShader = new FragmentShader();
    fShader->source(ShaderSource::fromCString(fragmentShaderSource));
    fShader->compile();
    
    // link shaders
//    int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    // check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
    
    Program * prog = new Program();
    prog->attach(vShader)
            ->attach(fShader)
            ->link();
    
    delete vShader;
    delete fShader;
    
    int shaderProgram = prog->program();
    
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
    
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//    glBindVertexArray(VAO);
    
    VertexArray * VA = new VertexArray();
    VA->bind();
    
    ArrayBuffer * VB = new ArrayBuffer();
    ElementArrayBuffer * EB = new ElementArrayBuffer();
    
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    VB->bind();
    VB->load(sizeof(vertices), vertices, GL_STATIC_DRAW);

    EB->bind();
    EB->load(sizeof(indices), indices, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
    
    VB->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
                                  ->index(0)
                                  ->size(3)
                                  ->type(GL_FLOAT)
                                  ->normalized(GL_FALSE)
                                  ->stride(3*sizeof(float))
                                  ->pointer((void *)0)
                                )->enable();
    
    VB->unbind();
//    EB->unbind();
    VA->unbind();
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0);
    
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!window->shouldClose())
    {
        // input
        // -----
        processInput(window);
        
        // render
        // ------
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
        
        GL::clear(new ColorCleaner(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)));
        
        // draw our first triangle
//        glUseProgram(shaderProgram);
        GL::useProgram(prog);
//        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
        VA->bind();
        
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GL::drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
        GLFW::swapBuffers(window);
        GLFW::pollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    delete VA;
//    delete VB;
//    delete EB;
    
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    GLFW::terminate();
    return 0;
}
