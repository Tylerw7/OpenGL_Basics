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


int main()
{
    std::cout << "Hello user";
    return 0;
}