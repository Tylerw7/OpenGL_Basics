#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

int main() {

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD AFTER creating and activating the OpenGL context
    // Glad and rendering will not work if this code is not here.
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Vertex shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;

        void main()
        {
            gl_Position = vec4(position.x, position.y, position.z, 1.0)
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Telling we want to create a shader
    const char* vertexShaderCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr); // 1.) our shader 2.) count of sources
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infolog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
        std::cerr << "ERROR:VERTEX_SHADER_COMPILATION_FAILED: " << infolog << std::endl;
    }

    // Fragment shader
    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        int main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infolog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
        std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " << infolog << std::endl;
    }

    // Bind the fragment and vertex shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infolog[512];
        glGetShaderInfoLog(shaderProgram, 512, nullptr, infolog);
        std::cerr << "ERROR:SHADER_PROGRAM_LINKED_FAILED: " << infolog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Our first triangle
    std::vector<float> vertices =
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Graphics pipeline
    // Vertex Data
    // Vertex Shader -> Primative Assembly -> Rasterization -> Fragment Shader -> Testing & Blending
    // Scren Buffer

    // The Fragment and Vertex Shader are what we will program



    // Program loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
};