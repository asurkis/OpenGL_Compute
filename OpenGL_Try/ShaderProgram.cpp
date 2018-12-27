#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram() :
    programId(glCreateProgram())
{}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programId);
}

void ShaderProgram::loadShader(GLenum type, const char *path)
{
    Shader *shader = new Shader(type, path);
    shader->load();
    shaders.push_back(shader);
}

void ShaderProgram::compile()
{
    for (auto shader: shaders)
        glAttachShader(programId, shader->id());

    glLinkProgram(programId);

    GLint result, logSize;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

    if (logSize > 0)
    {
        std::string log(logSize + 1, '\0');
        glGetProgramInfoLog(programId, logSize, NULL, &log[0]);
        std::cout << log << std::endl;
    }

    for (auto shader: shaders)
        glDetachShader(programId, shader->id());
}
