#pragma once

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <ostream>

/// @brief Class to manage all the initialization and infrastructure work before
/// we can draw
class context
{
public:
    context(std::ostream& errStream) : errStream(errStream)
    {
        if (!glfwInit())
        {
            errStream << "Could not initialize GLFW!\n";
            throw std::runtime_error("Could not initialize GLAD!");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    ~context() { glfwTerminate(); }

    GLFWwindow* createWindow(unsigned int width,
                             unsigned int height,
                             const char* name) const
    {
        GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
        if (window == NULL)
        {
            errStream << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, resizeCallback);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            errStream << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }

        return window;
    }

private:
    std::ostream& errStream;

    static void resizeCallback(GLFWwindow* window, int width, int height)
    {
        (void) window;
        glViewport(0, 0, width, height);
    }
};