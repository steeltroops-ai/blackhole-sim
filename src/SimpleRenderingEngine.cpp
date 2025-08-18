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
    // Create a high-quality sphere for better visibility
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Higher resolution for smoother spheres
    const int latSegments = 32;
    const int lonSegments = 64;

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

void renderBlackHoleEventHorizon(float x, float y, float z, float radius) {
    // Render the event horizon as a pure black sphere with glowing edge

    // 1. Render the main black sphere
    renderSphere(x, y, z, radius, 0.0f, 0.0f, 0.0f);

    // 2. Render glowing edge effect (slightly larger sphere with transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Outer glow - neon blue/cyan
    renderSphere(x, y, z, radius * 1.05f, 0.0f, 1.0f, 1.0f); // Cyan glow

    // Inner glow - electric blue
    renderSphere(x, y, z, radius * 1.02f, 0.2f, 0.6f, 1.0f); // Electric blue

    // Reset blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

void renderSpacetimeGrid(float size, int divisions) {
    // Create a dramatically curved grid showing Einstein's spacetime curvature
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float step = size / divisions;
    const float schwarzschildRadius = 6.0f; // Event horizon size

    // Generate grid vertices with realistic gravitational curvature
    for (int i = 0; i <= divisions; ++i) {
        for (int j = 0; j <= divisions; ++j) {
            float x = -size/2 + i * step;
            float z = -size/2 + j * step;

            // Calculate distance from black hole center
            float r = sqrt(x*x + z*z);
            float curvature = 0.0f;

            // Apply Schwarzschild metric curvature: stronger near event horizon
            if (r > schwarzschildRadius * 0.5f) {
                // Curvature follows 1/r² law but with dramatic visual scaling
                float curvatureFactor = schwarzschildRadius * schwarzschildRadius / (r * r);
                curvature = -curvatureFactor * 30.0f; // Scale for dramatic effect

                // Additional warping very close to black hole
                if (r < schwarzschildRadius * 2.0f) {
                    float proximityFactor = 1.0f - (r / (schwarzschildRadius * 2.0f));
                    curvature *= (1.0f + proximityFactor * 3.0f);
                }
            } else {
                // Inside photon sphere - extreme curvature
                curvature = -200.0f;
            }

            float y = curvature;

            // Anime-style glowing grid colors (teal to white gradient)
            float distanceNorm = r / (size * 0.5f);
            float intensity = exp(-distanceNorm * 1.5f); // Exponential falloff

            // Teal to white gradient with high intensity near black hole
            float r_color = 0.0f + intensity * 0.8f;      // Red component
            float g_color = 0.5f + intensity * 0.5f;      // Green component
            float b_color = 0.5f + intensity * 0.5f;      // Blue component (teal base)

            // Add pulsing effect for anime aesthetic
            static float time = 0.0f;
            time += 0.01f;
            float pulse = 0.8f + 0.2f * sin(time * 2.0f + r * 0.1f);

            r_color *= pulse;
            g_color *= pulse;
            b_color *= pulse;

            vertices.insert(vertices.end(), {x, y, z, r_color, g_color, b_color});
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

    // Enable blending for glowing lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glLineWidth(2.0f); // Thicker lines for better visibility
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    // Reset blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

void renderAccretionDisk(float innerRadius, float outerRadius) {
    // Create a realistic accretion disk with proper temperature gradients
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int segments = 128; // Higher resolution for smoother disk
    const int rings = 32;     // More rings for better gradient

    // Generate disk vertices with realistic physics-based coloring
    for (int ring = 0; ring <= rings; ++ring) {
        float radius = innerRadius + (outerRadius - innerRadius) * ring / rings;

        for (int seg = 0; seg <= segments; ++seg) {
            float angle = seg * 2.0f * M_PI / segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            float y = 0.0f; // Disk is in XZ plane

            // Physics-based temperature calculation: T ∝ r^(-3/4)
            float r_ratio = radius / innerRadius;
            float temperature = pow(r_ratio, -0.75f); // Shakura-Sunyaev model

            // Convert temperature to realistic colors
            float r, g, b;
            if (temperature > 0.8f) {
                // Very hot - blue-white (inner regions)
                r = 0.8f + 0.2f * temperature;
                g = 0.9f + 0.1f * temperature;
                b = 1.0f;
            } else if (temperature > 0.5f) {
                // Hot - white to yellow
                r = 1.0f;
                g = 1.0f;
                b = 0.3f + 0.7f * temperature;
            } else if (temperature > 0.2f) {
                // Warm - yellow to orange
                r = 1.0f;
                g = 0.4f + 0.6f * temperature;
                b = 0.1f + 0.2f * temperature;
            } else {
                // Cool - red (outer regions)
                r = 0.8f + 0.2f * temperature;
                g = 0.1f + 0.3f * temperature;
                b = 0.05f;
            }

            // Add brightness variation for more realism
            float brightness = 0.7f + 0.3f * temperature;
            r *= brightness;
            g *= brightness;
            b *= brightness;

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

    // Enable additive blending for glowing effect
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
        // Clear screen with deep space background
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Deep space blue-black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing for proper 3D rendering
        glEnable(GL_DEPTH_TEST);

        // Use shader program
        glUseProgram(g_shaderProgram);

        // Update camera position for orbital movement
        static float time = 0.0f;
        time += 0.01f;

        // Orbital camera movement around the black hole
        float radius = 60.0f; // Closer for better view
        g_cameraX = radius * cos(time * 0.15f);
        g_cameraZ = radius * sin(time * 0.15f);
        g_cameraY = 15.0f + 8.0f * sin(time * 0.08f);

        // Always look toward the black hole
        g_cameraYaw = -90.0f + time * 0.15f * 180.0f / M_PI;
        g_cameraPitch = -8.0f + 4.0f * sin(time * 0.08f);

        // Set up matrices
        float model[16], view[16], projection[16];
        createIdentityMatrix(model);
        createViewMatrix(view, g_cameraX, g_cameraY, g_cameraZ, g_cameraYaw, g_cameraPitch);
        createPerspectiveMatrix(projection, 50.0f * M_PI / 180.0f, (float)g_windowWidth / g_windowHeight, 0.1f, 1000.0f);

        // Set uniforms
        GLint modelLoc = glGetUniformLocation(g_shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(g_shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(g_shaderProgram, "projection");
        GLint alphaLoc = glGetUniformLocation(g_shaderProgram, "alpha");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

        // 1. Render background starfield first
        glUniform1f(alphaLoc, 0.8f);
        renderStarField();

        // 2. Render spacetime curvature grid (enhanced)
        glUniform1f(alphaLoc, 0.7f);
        renderSpacetimeGrid(120.0f, 24);

        // 3. Render enhanced accretion disk
        glUniform1f(alphaLoc, 0.9f);
        renderAccretionDisk(8.0f, 35.0f); // Larger, more visible disk

        // 4. Render the black hole event horizon with glowing edge
        glUniform1f(alphaLoc, 1.0f);
        renderBlackHoleEventHorizon(0.0f, 0.0f, 0.0f, 6.0f); // Proper Schwarzschild radius

        // 5. Render enhanced orbiting particles
        float particleTime = time * 1.5f;

        // Particle 1 - Close stable orbit (bright green)
        float orbit1 = 15.0f;
        float x1 = orbit1 * cos(particleTime);
        float z1 = orbit1 * sin(particleTime);
        renderSphere(x1, 1.0f, z1, 1.2f, 0.0f, 1.0f, 0.2f); // Bright green

        // Particle 2 - Elliptical orbit (orange)
        float orbit2a = 25.0f, orbit2b = 18.0f;
        float x2 = orbit2a * cos(particleTime * 0.6f);
        float z2 = orbit2b * sin(particleTime * 0.6f);
        renderSphere(x2, 2.0f, z2, 1.2f, 1.0f, 0.5f, 0.0f); // Orange

        // Particle 3 - Distant orbit (electric blue)
        float orbit3 = 45.0f;
        float x3 = orbit3 * cos(particleTime * 0.25f);
        float z3 = orbit3 * sin(particleTime * 0.25f);
        renderSphere(x3, -3.0f, z3, 1.2f, 0.2f, 0.8f, 1.0f); // Electric blue

        // 6. Render particle trajectories
        glUniform1f(alphaLoc, 0.6f);
        renderParticleTrajectory(x1, 1.0f, z1, x1 + 8.0f, 1.0f, z1 + 8.0f, 0.0f, 1.0f, 0.2f);
        renderParticleTrajectory(x2, 2.0f, z2, x2 + 12.0f, 2.0f, z2 + 12.0f, 1.0f, 0.5f, 0.0f);
        renderParticleTrajectory(x3, -3.0f, z3, x3 + 18.0f, -3.0f, z3 + 18.0f, 0.2f, 0.8f, 1.0f);

        // 7. Render gravitational lensing light rays
        glUniform1f(alphaLoc, 0.8f);
        glLineWidth(3.0f);

        // Multiple light rays showing lensing effects
        float lightTime = time * 2.0f;
        for (int i = 0; i < 5; ++i) {
            float angle = lightTime + i * 2.0f * M_PI / 5.0f;
            float startX = 70.0f * cos(angle);
            float startZ = 70.0f * sin(angle);
            float endX = -70.0f * cos(angle);
            float endZ = -70.0f * sin(angle);

            renderParticleTrajectory(startX, 0.0f, startZ, endX, 0.0f, endZ, 1.0f, 1.0f, 0.8f);
        }

        // Swap buffers
        glfwSwapBuffers(g_window);
        glfwPollEvents();

    } catch (...) {
        // If anything fails, just clear and swap
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
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
