#include "Shader.h"

#include <iostream>
#include <fstream>

Shader::Shader(GLenum type, const char *path) :
    type(type),
    path(path),
    shaderId(glCreateShader(type))
{}

Shader::~Shader()
{
    glDeleteShader(shaderId);
}

void Shader::load()
{
    readFile();
    compile();
}

void Shader::readFile()
{
    std::ifstream fin(path);
    if (!fin) throw std::exception();
    fin.seekg(0, std::ios::end);
    text.reserve(fin.tellg());
    fin.seekg(0, std::ios::beg);
    text.assign(
        std::istreambuf_iterator<char>(fin),
        std::istreambuf_iterator<char>());
}

void Shader::compile()
{
    std::cout << "Compiling shader " << path << std::endl;
    const char *src = text.c_str();
    glShaderSource(shaderId, 1, &src, NULL);
    glCompileShader(shaderId);

    GLint result, logSize;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

    if (logSize > 0)
    {
        std::string log(logSize + 1, '\0');
        glGetShaderInfoLog(shaderId, logSize, NULL, &log[0]);
        std::cout << &log[0] << std::endl;
    }
}
