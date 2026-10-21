#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>


float triangleData[] = {
    0,1,0, 1,0,0,
    -1,-1,0, 0,1,0,
    1,-1,0, 0,0,1
};


int main() {

    // Check to see if glfw was initialized
    if (!glfwInit()) {
        std::cout << "GLFW INIT ERROR!";
        return 1;
    }

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #endif    

    // create a window
    GLFWwindow *window = glfwCreateWindow(640, 480, "First Window", NULL, NULL);

    if (!window) {
        std::cout << " Window Error!";
        return 1;
    }

    // Create context for the window
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "GLAD INIT ERROR!";
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
    }

    // Create Buffer
    GLuint  buffer = 0;
    glGenBuffers(1, &buffer); // The first argument is how many buffers to create, usually from an array.
    
    // Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT); // First clear the screen

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // To destroy the window and clean things up.
    glfwDestroyWindow(window);
    glfwTerminate();
};


