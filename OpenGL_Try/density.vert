#version 430 core

layout (location = 0) in vec2 vertexPosition_modelspace;

uniform vec2 windowSize;

void main()
{
    gl_Position = vec4(vertexPosition_modelspace / windowSize
            * vec2(min(windowSize.x, windowSize.y)), 0, 1);
}
