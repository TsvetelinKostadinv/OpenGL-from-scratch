#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "buffer/buffer.h"
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

    gpu_buffer vertBuffer;
    vertBuffer.loadData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                        GL_STATIC_DRAW);

    layout layout;
    layout.push<float>(3);  // position
    layout.push<float>(3);  // color

    gpu_buffer indBuffer;
    indBuffer.loadData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                       GL_STATIC_DRAW);

    vertex_array vertArray(vertBuffer, layout, &indBuffer);
    shader.useProgram();
    vertArray.enable();

    assert(shader.getActiveAttributesCount() == 2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned),
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