#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

// resises the GLFW window when OpenGL viewport is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

void processInput(GLFWwindow *window); // closed the window with the escape key

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

int main()
{
    glfwInit(); // initialize GLFW before using any of its functions

    // set the window optios before creating it
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set the OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // set the OpenGL profile to core

    GLFWwindow *window = glfwCreateWindow(1024, 720, "FirstOpenGLWindow", NULL, NULL); // created the window widget

    if (window == NULL)
    {
        std::cout<< "Failed to create a GLFW window \n.";
        glfwTerminate(); // closes GLFW instanse
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window); // make the window the current context

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<< "Failed to start GLAD \n";
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600); // set the OpenGL viewport

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // resises the window when the viewport is resized


    // provide vertex data, can be a file
    float vertices[] = {-0.5f, -0.5f, 0.0f, // bottom left
                         0.5f, -0.5f, 0.0f, // bottom right
                         0.0f, 0.5f, 0.0f}; // top center

    unsigned int VBO; // initialise a vertex buffer object
    glGenBuffers(1, &VBO); // generate a buffer object at the address of VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer object to the GL_ARRAY_BUFFER target
    
    // copy the vertex data into the buffer object (i assume thats where you would load a file)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // create a vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attaches the shader to the address of the source code
    glCompileShader(vertexShader); // compile the shader

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // check for shader compile errors

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<< "Vertex Shader compilation failed: " << infoLog << "\n";
    }
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // checks for any input
        processInput(window); // checks for escape key press

        // implements rendering commands for each frame
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // clears only the colour information

        // register events and swap frames
        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}