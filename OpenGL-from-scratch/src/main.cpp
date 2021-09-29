#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "context.h"
#include "gl_types.h"
#include "shader/visual_shader.h"
#include "util/vec.h"

void processInput(GLFWwindow* window);

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr char* WINDOW_NAME = "OpenGL-from-scratch";

constexpr vec3f CLEAR_COLOR(0.2f, 0.3f, 0.3f);

enum exit_codes
{
    BAD_SHADERS,
};

int main()
{
    context context(std::cerr);

    GLFWwindow* window =
        context.createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

    const visual_shader shader = []()
    {
        std::optional<visual_shader> optShader =
            visual_shader::makeShaderFromFiles(
                std::cerr, "./resources/shaders/basic.vs",
                "./resources/shaders/basic.fs", std::nullopt);
        if (!optShader.has_value())
        {
            std::quick_exit(exit_codes::BAD_SHADERS);
        }
        // forced to move before return, because we don't have a copy
        // constructor
        return std::move(optShader).value();
    }();

    float vertices[] = {
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  //
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  //
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f,  //
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, gl_type<float>::gl_variant, GL_FALSE,
                          2 * sizeof(vec3f), (void*) 0);
    glVertexAttribPointer(1, 3, gl_type<float>::gl_variant, GL_FALSE,
                          2 * sizeof(vec3f), (void*) sizeof(vec3f));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    shader.useProgram();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int),
                       GL_UNSIGNED_INT, (void*) 0);
        assert(glGetError() == GL_NO_ERROR);

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