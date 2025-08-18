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
    // Create a simple sphere using triangulated icosphere
    const int subdivisions = 2;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate icosphere vertices and indices
    // For simplicity, create a basic sphere with latitude/longitude approach
    const int latSegments = 16;
    const int lonSegments = 32;

    for (int lat = 0; lat <= latSegments; ++lat) {
        float theta = lat * M_PI / latSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= lonSegments; ++lon) {
            float phi = lon * 2 * M_PI / lonSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float vx = x + radius * sinTheta * cosPhi;
            float vy = y + radius * cosTheta;
            float vz = z + radius * sinTheta * sinPhi;

            vertices.insert(vertices.end(), {vx, vy, vz, r, g, b});
        }
    }

    // Generate indices for triangles
    for (int lat = 0; lat < latSegments; ++lat) {
        for (int lon = 0; lon < lonSegments; ++lon) {
            int first = lat * (lonSegments + 1) + lon;
            int second = first + lonSegments + 1;

            indices.insert(indices.end(), {first, second, first + 1});
            indices.insert(indices.end(), {second, second + 1, first + 1});
        }
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void renderSpacetimeGrid(float size, int divisions) {
    // Create a curved grid to show spacetime curvature
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float step = size / divisions;

    // Generate grid vertices with curvature effect near center (black hole)
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float x = -size/2 + i * step;
            float z = -size/2 + j * step;

            // Calculate distance from center for curvature effect
            float dist = sqrt(x*x + z*z);
            float curvature = 0.0f;

            // Apply gravitational curvature (stronger near center)
            if (dist > 0.1f) {
                curvature = -20.0f / (dist + 5.0f); // Curved spacetime effect
            }

            float y = curvature;

            // Grid color - brighter near center, dimmer at edges
            float intensity = 0.3f + 0.4f * exp(-dist * 0.1f);
            vertices.insert(vertices.end(), {x, y, z, 0.2f, 0.6f, intensity});
        }
    }

    // Generate line indices for grid
    for (int i = 0; i < divisions; ++i) {
        for (int j = 0; j < divisions; ++j) {
            int idx = i * (divisions + 1) + j;

            // Horizontal lines
            if (j < divisions) {
                indices.insert(indices.end(), {idx, idx + 1});
            }

            // Vertical lines
            if (i < divisions) {
                indices.insert(indices.end(), {idx, idx + divisions + 1});
            }
        }
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glLineWidth(1.0f);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void renderAccretionDisk(float innerRadius, float outerRadius) {
    // Create a glowing accretion disk
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int segments = 64;
    const int rings = 16;

    // Generate disk vertices
    for (int ring = 0; ring <= rings; ++ring) {
        float radius = innerRadius + (outerRadius - innerRadius) * ring / rings;

        for (int seg = 0; seg <= segments; ++seg) {
            float angle = seg * 2.0f * M_PI / segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            float y = 0.0f; // Disk is in XZ plane

            // Color based on temperature (hotter = more blue/white, cooler = more red)
            float temp = 1.0f - (radius - innerRadius) / (outerRadius - innerRadius);
            float r = 1.0f;
            float g = 0.6f + 0.4f * temp;
            float b = 0.2f + 0.8f * temp;

            vertices.insert(vertices.end(), {x, y, z, r, g, b});
        }
    }

    // Generate triangle indices for disk
    for (int ring = 0; ring < rings; ++ring) {
        for (int seg = 0; seg < segments; ++seg) {
            int curr = ring * (segments + 1) + seg;
            int next = curr + segments + 1;

            indices.insert(indices.end(), {curr, next, curr + 1});
            indices.insert(indices.end(), {next, next + 1, curr + 1});
        }
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    // Enable blending for glowing effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Reset blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderParticleTrajectory(float startX, float startY, float startZ, float endX, float endY, float endZ, float r, float g, float b) {
    // Render a curved trajectory line showing particle path
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int segments = 32;

    // Generate curved path points
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / segments;

        // Linear interpolation with gravitational curve
        float x = startX + t * (endX - startX);
        float y = startY + t * (endY - startY);
        float z = startZ + t * (endZ - startZ);

        // Add gravitational curvature effect
        float distFromCenter = sqrt(x*x + y*y + z*z);
        if (distFromCenter > 1.0f) {
            float curvature = 5.0f / (distFromCenter * distFromCenter);
            // Bend trajectory toward center
            x *= (1.0f - curvature * t);
            z *= (1.0f - curvature * t);
        }

        // Color fades along trajectory
        float alpha = 1.0f - t * 0.5f;
        vertices.insert(vertices.end(), {x, y, z, r * alpha, g * alpha, b * alpha});

        if (i > 0) {
            indices.insert(indices.end(), {i-1, i});
        }
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glLineWidth(2.0f);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void renderStarField() {
    // Render background stars
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate random star positions
    const int numStars = 200;
    static bool starsGenerated = false;
    static std::vector<float> starPositions;

    if (!starsGenerated) {
        for (int i = 0; i < numStars; ++i) {
            // Random positions on a large sphere
            float theta = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
            float phi = ((float)rand() / RAND_MAX) * M_PI;
            float radius = 200.0f + ((float)rand() / RAND_MAX) * 100.0f;

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            float brightness = 0.3f + ((float)rand() / RAND_MAX) * 0.7f;
            starPositions.insert(starPositions.end(), {x, y, z, brightness, brightness, brightness});
        }
        starsGenerated = true;
    }

    // Use the pre-generated star positions
    vertices = starPositions;
    for (int i = 0; i < numStars; ++i) {
        indices.push_back(i);
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glPointSize(2.0f);
    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
}

extern "C" void renderSimpleBlackHole() {
    if (!g_initialized) return;

    try {
        // Clear screen with deep space black
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(g_shaderProgram);

        // Update camera position for orbital movement
        static float time = 0.0f;
        time += 0.01f;

        // Orbital camera movement around the black hole
        float radius = 80.0f;
        g_cameraX = radius * cos(time * 0.2f);
        g_cameraZ = radius * sin(time * 0.2f);
        g_cameraY = 20.0f + 10.0f * sin(time * 0.1f);

        // Always look toward the black hole
        g_cameraYaw = -90.0f + time * 0.2f * 180.0f / M_PI;
        g_cameraPitch = -10.0f + 5.0f * sin(time * 0.1f);

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

        // Start with simple rendering - just a few basic elements
        glUniform1f(alphaLoc, 1.0f);

        // Render black hole event horizon (simple dark sphere)
        std::vector<float> vertices = {
            0.0f, 0.0f, 0.0f, 0.05f, 0.05f, 0.05f  // Center black hole
        };
        std::vector<unsigned int> indices = {0};

        glBindVertexArray(g_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

        glPointSize(20.0f);
        glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);

        // Render some simple orbiting particles
        float particleTime = time * 2.0f;

        // Particle 1 - Close stable orbit
        float orbit1 = 12.0f;
        float x1 = orbit1 * cos(particleTime);
        float z1 = orbit1 * sin(particleTime);

        vertices = {x1, 0.5f, z1, 1.0f, 0.8f, 0.2f};
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glPointSize(8.0f);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

        // Particle 2 - Different orbit
        float orbit2 = 20.0f;
        float x2 = orbit2 * cos(particleTime * 0.7f);
        float z2 = orbit2 * sin(particleTime * 0.7f);

        vertices = {x2, 1.0f, z2, 0.2f, 1.0f, 0.8f};
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glPointSize(8.0f);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

        // Particle 3 - Distant orbit
        float orbit3 = 35.0f;
        float x3 = orbit3 * cos(particleTime * 0.3f);
        float z3 = orbit3 * sin(particleTime * 0.3f);

        vertices = {x3, -2.0f, z3, 0.8f, 0.2f, 1.0f};
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glPointSize(8.0f);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(g_window);
        glfwPollEvents();

    } catch (...) {
        // If anything fails, just clear and swap
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}

extern "C" bool shouldCloseWindow() {
    return g_window ? glfwWindowShouldClose(g_window) : true;
}

extern "C" GLFWwindow* getSimpleRendererWindow() {
    return g_window;
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
