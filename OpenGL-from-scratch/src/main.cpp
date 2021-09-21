#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "context.h"
#include "gl_types.h"
#include "util/vec.h"

void processInput(GLFWwindow* window);

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr char* WINDOW_NAME = "OpenGL-from-scratch";

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 inPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main()
{
    context context(std::cerr);

    GLFWwindow* window =
        context.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "[ERROR] Vertex shader compilation failed:\n"
                  << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "[ERROR] Fragment shader compilation failed:\n"
                  << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "[ERROR] Program linking failed:\n"
                  << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    vec3f vertices[] = {{-0.5f, -0.5f, 0.0f},  //
                        {0.5f, -0.5f, 0.0f},   //
                        {0.0f, 0.5f, 0.0f}};
    unsigned int VBO;
    unsigned int VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 3, vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(vertices) / sizeof(vec3f),
                          gl_type<float>::type, GL_FALSE, sizeof(vec3f),
                          (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vec3f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwDestroyWindow(window);

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}