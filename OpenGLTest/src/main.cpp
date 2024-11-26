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

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // check for shader compile errors
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<< "Fragment Shader compilation failed: " << infoLog << "\n";
    }

    unsigned int shaderProgram; // creates a shader program that combines all the shaders in one
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"Shader program link failed: " << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); // delete the shaders used to create the program

    // provide vertex data, can be a file
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };// top center
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO; // Vertex Array Object and Vertex Buffer Object, Element Buffer Obj
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // generate a buffer object at the address of VBO
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer object to the GL_ARRAY_BUFFER target
    // copy the vertex data into the buffer object (i assume thats where you would load a file)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // links EBO with whatever it links too
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // tell OpenGL the structure of the vertex
    glEnableVertexAttribArray(0);




    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // checks for any input
        processInput(window); // checks for escape key press

        // implements rendering commands for each frame
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // clears only the colour information

        glUseProgram(shaderProgram); // activate the created program
        glBindVertexArray(VAO); // binds VAO with VBO
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draws EBO

        // register events and swap frames
        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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