#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <MyShader.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

// resises the GLFW window when OpenGL viewport is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

void processInput(GLFWwindow *window); // closed the window with the escape key

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

    //MyShader shader_custom("src/shaders/vertexShader.vs", "src/shaders/fragmentShader.fs", true); // create a shader object
    MyShader shader_custom("src/shaders/vertexShader.vs", "src/shaders/fragmentShader.fs", true); // create a shader object

    // provide vertex data, can be a file
    float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right + Red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right + Green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left - Blue
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // top left
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

    // position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // tell OpenGL the structure of the vertex
    glEnableVertexAttribArray(0);
    // colour attr
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // checks for any input
        processInput(window); // checks for escape key press

        // implements rendering commands for each frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // clears only the colour information

        shader_custom.use(); // use the shader program

        // update the uniform color
        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO); // binds VAO with VBO
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draws EBO

        // register events and swap frames
        glfwSwapBuffers(window); 
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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