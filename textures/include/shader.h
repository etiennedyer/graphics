#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
    public:
    // program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. load shader code
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            vShaderFile.open(vertexPath); // open files
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream; // declare empty stringstream objects

            vShaderStream << vShaderFile.rdbuf();  // read data buffer
            fShaderStream << fShaderFile.rdbuf(); // i.e., convert the content into a stream
            
            vShaderFile.close();
            fShaderFile.close(); // close file handlers

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str(); // convert stream into string
        }
        catch(const std::ifstream::failure &e)
        {
            std::cout << "ERROR: shader file not succesfully read" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        //print any compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR: vertex shader compilation failed" << infoLog << std::endl;
        };

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        //print any compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR: fragment shader compilation failed" << infoLog << std::endl;
        };

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // print any linking errors
        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR: shader program linking failed" << infoLog << std::endl;
        }

        // delete shaders now that they're linked to the program and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    // declare that use() is a public method
    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const  //const tells us this is a read-only function
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);        
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};

#endif

