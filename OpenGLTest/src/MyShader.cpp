#include "MyShader.h"
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

MyShader::MyShader(const char* _vertexPath, const char* _fragmentPath, bool _isFile) : m_vxshader{_vertexPath}, m_frshader{_fragmentPath}, m_isFile{_isFile}
{
    // PART ONE: READ THE SHADER INPUT FILES
    // set up variables before reading the data from file
    std::string vertexCode;
    std::string fragmentCode;
    const char* vShaderCode;
    const char* fShaderCode;

    if (m_isFile)
    {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::cout << "Trying to open vertex shader file: " << _vertexPath << "\n";
            vShaderFile.open(_vertexPath);
            std::cout << "Vertex shader file opened\n";

            std::cout << "Trying to open fragment shader file: " << _fragmentPath << "\n";
            fShaderFile.open(_fragmentPath);
            std::cout << "Fragment shader file opened\n";

            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf(); // reads the file's buffer into variables
            std::cout<<"File read\n";

            vShaderFile.close();
            fShaderFile.close(); // closes the file after reading the data
            std::cout<<"File closed\n";

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str(); // converts the data into strings
            std::cout<<"File converted to string\n";
        
        }
        catch(std::ifstream::failure error)
        {
            std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"<< error.what() << "\n";
        }
        std::cout<<"File read and converted\n";
        vShaderCode = vertexCode.c_str();
        fShaderCode = fragmentCode.c_str(); // converts the strings into char arrays
        std::cout<<"File converted to char array\n";

    }

    else
    {
        vShaderCode = _vertexPath;
        fShaderCode = _fragmentPath; // stores things as is
    }
    
    // DONE READING THE FILE AND PROCESSING DATA
    // PART 2: COMPILE THE SHADER
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    std::cout<<"Compiling the vertex shader\n";
    // make the vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex); // compile the vertex shader
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // check for compile errors
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<infoLog<< "\n";
    }

    //make fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment); // compile the fragment shader
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); // check for compile errors

    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<infoLog<< "\n";
    }

    // VERTEX AND FRAGMENT SHADERS COMPILED
    // PART 3: CREATE THE SHADER PROGRAM

    ID = glCreateProgram(); // create the shader program
    glAttachShader(ID, vertex); // hooks up vertext shader to program
    glAttachShader(ID, fragment); // hooks up fragment shader to program
    glLinkProgram(ID); // links the program
    glGetProgramiv(ID, GL_LINK_STATUS, &success); // check for linking errors
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<infoLog<< "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment); // delete the shaders after linking

}

void MyShader::use()
{
    glUseProgram(ID);
}

void MyShader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void MyShader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void MyShader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}