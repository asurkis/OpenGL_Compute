#include "App.h"

#include <exception>

#define CALCULATE_FRAMES 128

App::App()
{}

App::~App()
{
    glDeleteBuffers(4, bufferIdx);
    glDeleteVertexArrays(2, vertexArrayIdx);

    delete computeProgram;
    delete particleProgram;

    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run()
{
    init();

    glfwSwapInterval(0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 2; i++)
    {
        glBindVertexArray(vertexArrayIdx[i]);
        glEnableVertexAttribArray(0);
    }

    computeProgram->use();
    GLint dtLocation = computeProgram->uniformLocation("dt");
    glUniform1f(dtLocation, 1e-4f);

    particleProgram->use();
    GLint windowSizeLocationParticles = particleProgram->uniformLocation("windowSize");
    glUniform2f(windowSizeLocationParticles, 1.0f, 1.0f);

#ifdef CALCULATE_FRAMES
    int frameNumber = 0;
    double frameTime[CALCULATE_FRAMES] = { 0.0 };
    double sumTime = 0.0;
#endif

    double time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double prevTime = time;
        time = glfwGetTime();

#ifdef CALCULATE_FRAMES
        frameTime[frameNumber] = time - prevTime;
        sumTime += time - prevTime;
        sumTime -= frameTime[(frameNumber + 1) % CALCULATE_FRAMES];
        ++frameNumber %= CALCULATE_FRAMES;

        char buf[256];
        sprintf_s(buf, "FPS: %.1f", (float) (CALCULATE_FRAMES / sumTime));
        glfwSetWindowTitle(window, buf);
#endif

        glClear(GL_COLOR_BUFFER_BIT);

        computeProgram->use();
        glUniform1f(dtLocation, (GLfloat) (time - prevTime));
        glDispatchCompute(PARTICLE_COUNT / 64, 1, 1);

        particleProgram->use();
        glBindVertexArray(vertexArrayIdx[0]);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (int i = 0; i < 2; i++)
    {
        glBindVertexArray(vertexArrayIdx[i]);
        glDisableVertexAttribArray(0);
    }
}

void errorCallback(int error, const char *description)
{
    fprintf_s(stderr, "GLFW error #%d: \"%s\"", error, description);
}

void App::handleResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    glUniform2f(glGetUniformLocation(particleProgram->id(), "windowSize"),
                (GLfloat)width, (GLfloat)height);
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
    app.handleResize(window, width, height);
}

void App::handleKey(GLFWwindow *window, int key, int scancode, int action, int modifiers)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, 1);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int modifiers)
{
    app.handleKey(window, key, scancode, action, modifiers);
}

void App::init()
{
    initGLFW();
    initGLEW();
    initBuffers();
    initShaders();
}

void App::initGLFW()
{
    if (!glfwInit())
        throw std::exception();

    glfwSetErrorCallback(&errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window1", NULL, NULL);
    if (!window)
        throw std::exception();

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);

    const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwSetWindowPos(window, (vidmode->width - windowWidth) / 2, (vidmode->height - windowHeight) / 2);
}

void App::initGLEW()
{
    if (glewInit() != GLEW_OK)
        throw std::exception();
}

void App::initBuffers()
{
    glGenVertexArrays(1, vertexArrayIdx);

    glGenBuffers(2, bufferIdx);

    GLfloat *bufferData = new GLfloat[2 * PARTICLE_COUNT];
    for (int i = 0; i < 2 * PARTICLE_COUNT; i++)
        bufferData[i] = 2.0f * random() - 1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, bufferIdx[0]);
    glBufferData(GL_ARRAY_BUFFER, 2 * PARTICLE_COUNT * sizeof(GLfloat), bufferData, GL_DYNAMIC_DRAW);

    glBindVertexArray(vertexArrayIdx[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    for (int i = 0; i < 2 * PARTICLE_COUNT; i++)
        bufferData[i] = 0.0f;

    glBindBuffer(GL_ARRAY_BUFFER, bufferIdx[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * PARTICLE_COUNT * sizeof(GLfloat), bufferData, GL_DYNAMIC_DRAW);
    delete[] bufferData;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (int i = 0; i < 2; i++)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, bufferIdx[i]);
}

void App::initShaders()
{
    computeProgram = new ShaderProgram;
    computeProgram->loadShader(GL_COMPUTE_SHADER, "interactions.comp");
    computeProgram->compile();

    particleProgram = new ShaderProgram;
    particleProgram->loadShader(GL_VERTEX_SHADER, "particles.vert");
    particleProgram->loadShader(GL_FRAGMENT_SHADER, "particles.frag");
    particleProgram->compile();
}

float App::random()
{
    return (float) rand() / (float) RAND_MAX;
}

App app;

int main()
{
    app.run();
}
