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



// Simple vector structure
struct Vec2
{
    float x,y;
    Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Vec2i
{
    int x, y;
    Vec2i(int x = 0, int y = 0) : x(x), y(y) {}
};

struct Vec3
{
    float r, g, b;
    Vec3() : r(0.0f), g(0.0f), b(0.0f) {}
    Vec3(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Game Constants
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const float UPDATE_INTERVAL = 0.15f; // seconds
const float CELL_WIDTH = 2.0f / GRID_WIDTH;
const float CELL_HEIGHT = 2.0f / GRID_HEIGHT;


// Game state
enum class Direction
{
    Up,
    Down,
    Left,
    Right,
    None
};
Direction snakeDir = Direction::None; // The initial direction of the snake
std::vector<Vec2i> snake = { Vec2i(5, 10), Vec2i(4, 10), Vec2i(3, 10) };
Vec2i fruit;
int score = 0;
bool gameOver = false;
bool gameStarted = false;
float timeSinceLastUpdate = 0.0f;
float snakeSpeed = UPDATE_INTERVAL;


// Shader sources
std::string vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    uniform vec2 uOffset;
    uniform vec2 uScale;
    
    void main() {
        vec2 position = (aPos * uScale) + uOffset;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

std::string fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 uColor;
    
    void main() {
        FragColor = vec4(uColor, 1.0);
    }
)";


// OpenGL objects
GLuint shaderProgram;
GLuint VAO, VBO;
GLuint uOffsetLoc, uScaleLoc, uColorLoc;

// Bitmap font - each character is 5x5 pixels
const int FONT_WIDTH = 5;
const int FONT_HEIGHT = 5;
const int FONT_SPACING = 1;

// Character definitions (0 = empty, 1 = filled)
std::map<char, std::vector<int>> fontMap = {
    {' ', {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}},
    {'A', {0,1,1,0,0, 1,0,0,1,0, 1,1,1,1,0, 1,0,0,1,0, 1,0,0,1,0}},
    {'B', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0}},
    {'C', {0,1,1,1,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 0,1,1,1,0}},
    {'D', {1,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 1,1,1,0,0}},
    {'E', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,0,0, 1,1,1,1,0}},
    {'F', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,0,0, 1,0,0,0,0}},
    {'G', {0,1,1,1,0, 1,0,0,0,0, 1,0,1,1,0, 1,0,0,1,0, 0,1,1,1,0}},
    {'H', {1,0,0,1,0, 1,0,0,1,0, 1,1,1,1,0, 1,0,0,1,0, 1,0,0,1,0}},
    {'I', {1,1,1,0,0, 0,1,0,0,0, 0,1,0,0,0, 0,1,0,0,0, 1,1,1,0,0}},
    {'J', {0,0,1,1,0, 0,0,0,1,0, 0,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'K', {1,0,0,1,0, 1,0,1,0,0, 1,1,0,0,0, 1,0,1,0,0, 1,0,0,1,0}},
    {'L', {1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,0}},
    {'M', {1,0,0,0,1, 1,1,0,1,1, 1,0,1,0,1, 1,0,0,0,1, 1,0,0,0,1}},
    {'N', {1,0,0,0,1, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,0,0,0,1}},
    {'O', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'P', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,0,0, 1,0,0,0,0}},
    {'Q', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,1,0,0, 0,1,0,1,0}},
    {'R', {1,1,1,0,0, 1,0,0,1,0, 1,1,1,0,0, 1,0,1,0,0, 1,0,0,1,0}},
    {'S', {0,1,1,1,0, 1,0,0,0,0, 0,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'T', {1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {'U', {1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'V', {1,0,0,0,1, 1,0,0,0,1, 0,1,0,1,0, 0,1,0,1,0, 0,0,1,0,0}},
    {'W', {1,0,0,0,1, 1,0,0,0,1, 1,0,1,0,1, 1,0,1,0,1, 0,1,0,1,0}},
    {'X', {1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,1,0,1,0, 1,0,0,0,1}},
    {'Y', {1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}},
    {'Z', {1,1,1,1,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,1}},
    {'0', {0,1,1,0,0, 1,0,0,1,0, 1,0,0,1,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'1', {0,0,1,0,0, 0,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,1,0}},
    {'2', {0,1,1,0,0, 1,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,0}},
    {'3', {1,1,1,0,0, 0,0,0,1,0, 0,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'4', {0,0,1,1,0, 0,1,0,1,0, 1,0,0,1,0, 1,1,1,1,1, 0,0,0,1,0}},
    {'5', {1,1,1,1,0, 1,0,0,0,0, 1,1,1,0,0, 0,0,0,1,0, 1,1,1,0,0}},
    {'6', {0,1,1,0,0, 1,0,0,0,0, 1,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'7', {1,1,1,1,0, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,0,0,0,0}},
    {'8', {0,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0, 1,0,0,1,0, 0,1,1,0,0}},
    {'9', {0,1,1,0,0, 1,0,0,1,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,0,0}},
    {':', {0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0}},
    {'-', {0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0}},
    {'.', {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0}}
};


// Function declarations
void SpawnFruit();
void InitGame();
void ResetGame();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void DrawCell(const Vec2i& position, const Vec3& color);
void DrawChar(char c, float x, float y, float scale, const Vec3& color);
void DrawText(const std::string& text, float x, float y, float scale, const Vec3& color);
void RenderGame(GLFWwindow* window);
void UpdateGame(float deltaTime);
void DrawBorder();
void DrawSnake();
void DrawScore();
void DrawGameOver();
void DrawStartScreen();


int main()
{
    #if defined (__linux__)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    #endif

    if (!glfwInit())
    {
        std::cout << "Error initialising GLFW" << std::endl;
        return 1;
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


    // Create the window
    GLFWwindow* window = glfwCreateWindow(800,800, "SnakeGame", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Give the window a context
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);

    // Initialize GLAD AFTER creating and activating the OpenGL context
    // Glad and rendering will not work if this code is not here.
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Compile the  Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCStr = vertexShaderSource.c_str(); // converte to C string
    glShaderSource(vertexShader, 1, & vertexShaderCStr, NULL);
    glCompileShader(vertexShader);

    // Check vertex Shader Comilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR:VERTEX_SHADER_COMILATION_FAILED" << infoLog << std::endl;
        glfwTerminate();
        return 1;
    }

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check shader program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR:SHADER_PROGRAM_LINKING_FAILED: " << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Get uniform locations
    uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");
    uColorLoc = glGetUniformLocation(shaderProgram, "uColor");

    // Setup quad VAO
    const float vertices[] =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return 0;
}