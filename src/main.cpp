#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>


float triangleData[] = {
    0,1,0, 1,0,0,
    -1,-1,0, 0,1,0,
    1,-1,0, 0,0,1
};

// Vertex shader
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    vertexColor = aColor;
}
)";

// Fragment shader: determines the color of each pixel.
const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Compile a shader and report errors.
GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n"
                  << infoLog << '\n';

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Compile both shaders and link them into a program.
GLuint createShaderProgram()
{
    GLuint vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexShaderSource);

    GLuint fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0) {
        if (vertexShader != 0)
            glDeleteShader(vertexShader);

        if (fragmentShader != 0)
            glDeleteShader(fragmentShader);

        return 0;
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader linking error:\n"
                  << infoLog << '\n';

        glDeleteProgram(program);
        program = 0;
    }

    // The linked program no longer needs the shader objects.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}




// MAIN FUNCTION
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

    // Create shader
    GLuint shaderProgram = createShaderProgram();

    if (shaderProgram == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }


    // Create a VAO to store vertex attribute configuration.
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the vertex buffer.
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

    // The Attributes Representing Position
    glEnableVertexAttribArray(0); // first attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); // starting index, number of items, float

    // The Attribute For Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));



    // Main loop
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT); // First clear the screen

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES,0,3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Clean up GPU resources
    glDeleteBuffers(1,&VAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    // To destroy the window and clean things up.
    glfwDestroyWindow(window);
    glfwTerminate();
};


