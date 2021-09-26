#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "context.h"
#include "gl_types.h"
#include "util/vec.h"
#include "visual_shader.h"

void processInput(GLFWwindow* window);

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr char* WINDOW_NAME = "OpenGL-from-scratch";

int main()
{
    context context(std::cerr);

    GLFWwindow* window =
        context.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    std::optional<visual_shader> optShader = visual_shader::makeShaderFromFiles(
        std::cerr, "./resources/shaders/basic.vs",
        "./resources/shaders/basic.fs", std::nullopt);
    if (!optShader.has_value())
    {
        return 1;
    }
    const visual_shader shader = std::move(optShader).value();

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
                          gl_type<float>::gl_variant, GL_FALSE, sizeof(vec3f),
                          (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    shader.useProgram();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vec3f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}