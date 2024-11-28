#ifndef MYSHADER_H
#define MYSHADER_H // this prevents the header file from being included more than once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class MyShader
{
    public:
        // the programs id
        unsigned int ID;

        // constructor reads and builds the shader
        MyShader(const char* _vertexPath, const char* _fragmentPath, bool _isFile = false); 

        void use(); // activate shader
        void setBool(const std::string &name, bool value) const; // unmutable func  that changes the bool status
        void setInt(const std::string &name, int value) const; // unmutable func that changes the int status
        void setFloat(const std::string &name, float value) const; // unmutable func that changes the float status

    private:
        const char* m_vxshader;
        const char* m_frshader;
        bool m_isFile;
};

#endif