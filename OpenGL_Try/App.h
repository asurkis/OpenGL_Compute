#pragma once

#include "ShaderProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App
{
    GLFWwindow *window;
    GLuint vertexArrayIdx[1];
    GLuint bufferIdx[2];

    ShaderProgram *computeProgram;
    ShaderProgram *particleProgram;

    void init();

    void initGLFW();
    void initGLEW();
    void initBuffers();
    void initShaders();

    float random();

    App(const App &) {}

public:
    App();
    ~App();
    void run();

    void handleKey(GLFWwindow *window, int key, int scancode, int action, int modifiers);
    void handleResize(GLFWwindow *window, int width, int height);

    inline GLFWwindow *getWindow() const { return window; }

    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 800;
    static const int IMAGE_SIZE = 400;
    static const int PARTICLE_COUNT = 1 << 15;
};

extern App app;
