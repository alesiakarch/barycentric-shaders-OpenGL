#include <gtest/gtest.h>
//#include <GLFW/glfw3.h>
#include <MyShader.h>
#include <fstream>
#include <sstream>




// Helper function to create a temporary file with given content
std::string createTempFile(const char* content) 
{
    std::string fileName = "src/temp_shader_" + std::to_string(rand()) + ".glsl";
    std::ofstream outFile(fileName);
    outFile << content;
    outFile.close();
    return fileName;
}

// TEST(MyShader, ctor_fail) // tests the class constructor with invalid string
// {
//     MyShader shader("invalidShaderSource", "invalidShaderSource");
//     EXPECT_EQ(shader.ID, 0);
// }


TEST(MyShader, ctor_pass)
{
    const char* vertexShaderSource = "#version 330 core\n" // test shader source
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n" // test shader source
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n\0";
    
    // glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::string validVShaderFile = createTempFile(vertexShaderSource);
    std::string validFShaderFile = createTempFile(fragmentShaderSource);
    std::cout << "Vertex shader file: " << validVShaderFile << std::endl;
    const char* v_path = validVShaderFile.c_str();
    const char* f_path = validFShaderFile.c_str();
    MyShader shader(v_path, f_path, true);
    EXPECT_NE(shader.ID, 0);

    // Clean up
    remove(validVShaderFile.c_str());
    remove(validFShaderFile.c_str());
}
TEST(MyShader, use)
{
        
}