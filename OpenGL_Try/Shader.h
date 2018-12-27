#pragma once

#include <GL/glew.h>
#include <string>

class Shader
{
    GLenum type;
    const char *path;

    std::string text;

    GLuint shaderId;

    void readFile();
    void compile();

    Shader(const Shader &) {}

public:
    Shader(GLenum type, const char *path);
    ~Shader();
    void load();

    inline GLuint id() const { return shaderId; }
};
