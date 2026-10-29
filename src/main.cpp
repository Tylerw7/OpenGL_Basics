#define GLFW_INCLUDE_NONE 
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <map>



// ----------------------------------------------------------------
// SHADERS
// ----------------------------------------------------------------

// Vertex Shader
std::string vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// Fragment Shader
std::string fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";




int main()
{
    if (!glfwInit())
    {
        std::cout << "ERROR: GLFWINIT";
        return 1;
    }

    // Set up window hints for verson 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #endif    

    // Create Window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Snake_Game", nullptr, nullptr);
    if (!window) {
        std::cout << " Window Error!";
        glfwTerminate();
        return 1;
    }

    // Link window to context
    glfwMakeContextCurrent(window);

    // initiate GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "ERROR: FAILED TO LOAD GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Create the Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSourceC_Str = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceC_Str, nullptr);
    glCompileShader(vertexShader);


    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}