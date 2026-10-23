#define GLFW_INCLUDE_NONE // This macro will help the include order erro from happening with glad and glfw
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>


struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

Vec2 offset;




void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                std::cout << "GLFW_KEY_UP" << std::endl;
                offset.y += 0.01f;
                break;
            case GLFW_KEY_DOWN:
                std::cout << "GLFW_KEY_DOWN" << std::endl; 
                offset.y -= 0.01f;  
                break;
            case GLFW_KEY_RIGHT:
                std::cout << "GLFW_KEY_RIGHT" << std::endl;
                offset.x += 0.01f;
                break;
            case GLFW_KEY_LEFT:
                std::cout << "GLFW_KEY_LEFT" << std::endl; 
                offset.x -= 0.01f; 
                break;
            default:
                break;         
        }
    }
}

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

    // Keyboard call back
    glfwSetKeyCallback(window, keyCallback);

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
        layout (location = 1) in vec3 color;
        
        uniform vec2 uOffset;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
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

        in vec3 vColor;
        uniform vec4 uColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0) * uColor;
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
        0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0,
        0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f 
    };

    std::vector<unsigned int> indices = 
    {
        0,1,2,
        0,2,3
    };

    // Graphics pipeline
    // Vertex Data
    // Vertex Shader -> Primative Assembly -> Rasterization -> Fragment Shader -> Testing & Blending
    // Scren Buffer

    // The Fragment and Vertex Shader are what we will program


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // Transfer vertex data from system memory to gpu memory
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Now our vertex data resides in gpu memory
    // Next tell shader program how to interprate the data when reach fragment shader


    // Create EBO
    GLuint ebo;
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT, false, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");



    // Program loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glUniform2f(uOffsetLoc, offset.x, offset.y);
        glBindVertexArray(vao); // bind vao
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
};