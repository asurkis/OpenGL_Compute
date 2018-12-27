#pragma once

#include "Shader.h"

#include <GL/glew.h>
#include <vector>

class ShaderProgram
{
    GLuint programId;
    std::vector<Shader *> shaders;

    ShaderProgram(const ShaderProgram &) {}

public:
    ShaderProgram();
    ~ShaderProgram();

    void loadShader(GLenum type, const char *path);
    void compile();

    inline GLuint id() const { return programId; }
    inline GLint uniformLocation(const GLchar *name) const { return glGetUniformLocation(programId, name); }
    inline void use() const { glUseProgram(programId); }
};
