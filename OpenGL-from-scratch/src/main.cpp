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

    float vertices[] = {
        0.5f,  0.5f,  0.0f,  //
        0.5f,  -0.5f, 0.0f,  //
        -0.5f, -0.5f, 0.0f,  //
        -0.5f, 0.5f,  0.0f   //
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 3, vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(vertices) / sizeof(vec3f),
                          gl_type<float>::gl_variant, GL_FALSE, sizeof(vec3f),
                          (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    shader.useProgram();

    float x, y, z;
    x = y = z = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        x += 0.0001f;
        y += 0.0002f;
        z += 0.0003f;
        shader.setUniform3f("color", x, y, z);
        // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vec3f));
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int),
                       GL_UNSIGNED_INT, (void*) 0);

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