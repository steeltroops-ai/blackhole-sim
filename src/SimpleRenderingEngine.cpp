#include "glad.h"
#include <GLFW/glfw3.h>
#include "RenderingEngine.hpp"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace BlackHoleSim;

// Simple vertex shader for 3D rendering
const char* simpleVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

// Simple fragment shader
const char* simpleFragmentShader = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

uniform float alpha;

void main() {
    FragColor = vec4(vertexColor, alpha);
}
)";

// Global variables for simplified implementation
static GLFWwindow* g_window = nullptr;
static GLuint g_shaderProgram = 0;
static GLuint g_VAO = 0, g_VBO = 0, g_EBO = 0;
static int g_windowWidth = 1280, g_windowHeight = 720;
static bool g_initialized = false;

// Camera variables
static float g_cameraX = 0.0f, g_cameraY = 0.0f, g_cameraZ = 50.0f;
static float g_cameraYaw = -90.0f, g_cameraPitch = 0.0f;

// Utility functions
void createIdentityMatrix(float* matrix) {
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void createPerspectiveMatrix(float* matrix, float fov, float aspect, float nearPlane, float farPlane) {
    float f = 1.0f / tan(fov * 0.5f);
    createIdentityMatrix(matrix);
    matrix[0] = f / aspect;
    matrix[5] = f;
    matrix[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
    matrix[15] = 0.0f;
}

void createViewMatrix(float* matrix, float x, float y, float z, float yaw, float pitch) {
    float cosYaw = cos(yaw * M_PI / 180.0f);
    float sinYaw = sin(yaw * M_PI / 180.0f);
    float cosPitch = cos(pitch * M_PI / 180.0f);
    float sinPitch = sin(pitch * M_PI / 180.0f);
    
    float frontX = cosYaw * cosPitch;
    float frontY = sinPitch;
    float frontZ = sinYaw * cosPitch;
    
    createIdentityMatrix(matrix);
    matrix[12] = -x;
    matrix[13] = -y;
    matrix[14] = -z;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return 0;
    }
    return shader;
}

extern "C" bool initializeSimpleRenderer() {
    if (g_initialized) return true;
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Create window
    g_window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Black Hole Simulation - 3D View", nullptr, nullptr);
    if (!g_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(g_window);
    
    // Initialize GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(g_window);
        glfwTerminate();
        return false;
    }
    
    // Enable VSync
    glfwSwapInterval(1);
    
    // Set viewport
    glViewport(0, 0, g_windowWidth, g_windowHeight);
    
    // Enable depth testing and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, simpleVertexShader);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, simpleFragmentShader);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        std::cerr << "Failed to compile shaders" << std::endl;
        return false;
    }
    
    // Create shader program
    g_shaderProgram = glCreateProgram();
    glAttachShader(g_shaderProgram, vertexShader);
    glAttachShader(g_shaderProgram, fragmentShader);
    glLinkProgram(g_shaderProgram);
    
    GLint success;
    glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(g_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &g_VAO);
    glGenBuffers(1, &g_VBO);
    glGenBuffers(1, &g_EBO);
    
    glBindVertexArray(g_VAO);
    
    // Set up vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    
    std::cout << "Simple renderer initialized successfully!" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    g_initialized = true;
    return true;
}

void renderSphere(float x, float y, float z, float radius, float r, float g, float b) {
    // Simple sphere rendering using points for now
    std::vector<float> vertices = {
        x, y, z, r, g, b
    };
    std::vector<unsigned int> indices = {0};
    
    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    glPointSize(radius * 10.0f);
    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
}

extern "C" void renderSimpleBlackHole() {
    if (!g_initialized) return;
    
    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader program
    glUseProgram(g_shaderProgram);
    
    // Set up matrices
    float model[16], view[16], projection[16];
    createIdentityMatrix(model);
    createViewMatrix(view, g_cameraX, g_cameraY, g_cameraZ, g_cameraYaw, g_cameraPitch);
    createPerspectiveMatrix(projection, 45.0f * M_PI / 180.0f, (float)g_windowWidth / g_windowHeight, 0.1f, 1000.0f);
    
    // Set uniforms
    GLint modelLoc = glGetUniformLocation(g_shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(g_shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(g_shaderProgram, "projection");
    GLint alphaLoc = glGetUniformLocation(g_shaderProgram, "alpha");
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
    glUniform1f(alphaLoc, 1.0f);
    
    // Render black hole (dark sphere at center)
    renderSphere(0.0f, 0.0f, 0.0f, 2.0f, 0.1f, 0.1f, 0.1f);
    
    // Render some test particles
    renderSphere(10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.8f, 0.2f);
    renderSphere(-10.0f, 5.0f, 0.0f, 1.0f, 0.8f, 1.0f, 0.2f);
    renderSphere(0.0f, -8.0f, 5.0f, 1.0f, 0.2f, 0.8f, 1.0f);
    
    // Swap buffers
    glfwSwapBuffers(g_window);
    glfwPollEvents();
}

extern "C" bool shouldCloseWindow() {
    return g_window ? glfwWindowShouldClose(g_window) : true;
}

extern "C" void cleanupSimpleRenderer() {
    if (g_VAO) glDeleteVertexArrays(1, &g_VAO);
    if (g_VBO) glDeleteBuffers(1, &g_VBO);
    if (g_EBO) glDeleteBuffers(1, &g_EBO);
    if (g_shaderProgram) glDeleteProgram(g_shaderProgram);
    
    if (g_window) {
        glfwDestroyWindow(g_window);
        glfwTerminate();
    }
    
    g_initialized = false;
}
