#include "glad.h"
#include "glfw_replacement.h"
#include "RenderingEngine.hpp"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace BlackHoleSim;

// Enhanced anime-style vertex shader
const char* simpleVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 vertexColor;
out vec3 worldPos;
out float distanceFromCenter;

void main() {
    vec4 worldPosition = model * vec4(aPos, 1.0);
    worldPos = worldPosition.xyz;
    distanceFromCenter = length(worldPos);

    gl_Position = projection * view * worldPosition;
    vertexColor = aColor;
}
)";

// Enhanced anime-style fragment shader with cel-shading and effects
const char* simpleFragmentShader = R"(
#version 330 core
in vec3 vertexColor;
in vec3 worldPos;
in float distanceFromCenter;

uniform float alpha;
uniform float time;

out vec4 FragColor;

// Anime-style color enhancement
vec3 enhanceColors(vec3 color) {
    // Increase saturation for vibrant anime look
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));
    vec3 saturated = mix(vec3(luminance), color, 1.4);

    // Add bloom effect for bright colors
    float brightness = max(max(saturated.r, saturated.g), saturated.b);
    if (brightness > 0.7) {
        saturated += vec3(0.15) * (brightness - 0.7) * 3.0;
    }

    return saturated;
}

// Cel-shading quantization
vec3 celShade(vec3 color) {
    // Quantize colors to create flat anime-style regions
    float levels = 5.0;
    vec3 quantized = floor(color * levels) / levels;

    // Add subtle edge enhancement
    float edgeFactor = smoothstep(0.6, 0.8, max(max(color.r, color.g), color.b));
    quantized += vec3(0.1) * edgeFactor;

    return quantized;
}

void main() {
    vec3 finalColor = vertexColor;

    // Apply anime-style color enhancement
    finalColor = enhanceColors(finalColor);

    // Apply cel-shading for flat anime look
    finalColor = celShade(finalColor);

    // Add pulsing glow for bright objects
    float brightness = max(max(finalColor.r, finalColor.g), finalColor.b);
    if (brightness > 0.6) {
        float pulse = 0.95 + 0.05 * sin(time * 3.0 + distanceFromCenter * 0.1);
        finalColor *= pulse;
    }

    // Add subtle screen-space glow
    float screenGlow = 1.0 + 0.03 * sin(time * 1.5);
    finalColor *= screenGlow;

    // Ensure colors stay in valid range
    finalColor = clamp(finalColor, 0.0, 1.0);

    FragColor = vec4(finalColor, alpha);
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

// Interactive control variables
static bool g_showGrid = true;
static bool g_showAccretionDisk = true;
static bool g_showParticles = true;
static bool g_showTrajectories = true;
static bool g_showLensing = true;
static bool g_showBlackHole = true;
static bool g_showStars = true;
static bool g_showEinsteinRing = true;
static bool g_autoCamera = true;
static bool g_showUI = true;

// Animation control
static bool g_paused = false;
static float g_animationSpeed = 1.0f;
static float g_blackHoleMass = 10.0f; // Solar masses

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
    
    // Window hints not needed for Windows API implementation
    
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
    
    // VSync handled by Windows API implementation
    
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

            indices.insert(indices.end(), {static_cast<unsigned int>(first), static_cast<unsigned int>(second), static_cast<unsigned int>(first + 1)});
            indices.insert(indices.end(), {static_cast<unsigned int>(second), static_cast<unsigned int>(second + 1), static_cast<unsigned int>(first + 1)});
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
                indices.insert(indices.end(), {static_cast<unsigned int>(idx), static_cast<unsigned int>(idx + 1)});
            }

            // Vertical lines
            if (i < divisions) {
                indices.insert(indices.end(), {static_cast<unsigned int>(idx), static_cast<unsigned int>(idx + divisions + 1)});
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

void renderStarField() {
    // Render background stars for cosmic atmosphere
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate random star positions (static for consistency)
    static bool starsGenerated = false;
    static std::vector<float> starVertices;
    static std::vector<unsigned int> starIndices;

    if (!starsGenerated) {
        srand(42); // Fixed seed for consistent star field

        for (int i = 0; i < 200; ++i) {
            // Random positions in a large sphere around the scene
            float x = (rand() / float(RAND_MAX) - 0.5f) * 200.0f;
            float y = (rand() / float(RAND_MAX) - 0.5f) * 200.0f;
            float z = (rand() / float(RAND_MAX) - 0.5f) * 200.0f;

            // Random star colors (white, blue, yellow, red)
            float brightness = 0.3f + (rand() / float(RAND_MAX)) * 0.7f;
            float r = brightness;
            float g = brightness;
            float b = brightness;

            // Some stars have color tints
            if (rand() % 4 == 0) {
                b *= 1.2f; // Blue stars
            } else if (rand() % 4 == 1) {
                r *= 1.2f; g *= 1.1f; // Yellow stars
            } else if (rand() % 4 == 2) {
                r *= 1.3f; g *= 0.8f; b *= 0.7f; // Red stars
            }

            starVertices.insert(starVertices.end(), {x, y, z, r, g, b});
            starIndices.push_back(i);
        }
        starsGenerated = true;
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, starIndices.size() * sizeof(unsigned int), starIndices.data(), GL_STATIC_DRAW);

    glPointSize(2.0f);
    glDrawElements(GL_POINTS, starIndices.size(), GL_UNSIGNED_INT, 0);
}

void renderParticleTrajectory(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b) {
    // Render a curved trajectory line between two points
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int segments = 20;

    // Create a curved path (simple bezier-like curve)
    for (int i = 0; i <= segments; ++i) {
        float t = float(i) / segments;

        // Linear interpolation with slight curve
        float x = x1 + (x2 - x1) * t;
        float y = y1 + (y2 - y1) * t + sin(t * M_PI) * 2.0f; // Add curve
        float z = z1 + (z2 - z1) * t;

        // Fade the trail
        float alpha = 1.0f - t * 0.5f;
        vertices.insert(vertices.end(), {x, y, z, r * alpha, g * alpha, b * alpha});

        if (i > 0) {
            indices.insert(indices.end(), {static_cast<unsigned int>(i-1), static_cast<unsigned int>(i)});
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

void renderGravitationalLensing(float blackHoleX, float blackHoleY, float blackHoleZ, float schwarzschildRadius) {
    // Render light rays showing gravitational lensing effects
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int numRays = 12;
    const int segmentsPerRay = 30;
    const float maxDistance = 80.0f;

    static float time = 0.0f;
    time += 0.02f;

    for (int ray = 0; ray < numRays; ++ray) {
        float angle = (ray * 2.0f * M_PI / numRays) + time * 0.1f;

        // Start position (far from black hole)
        float startX = maxDistance * cos(angle);
        float startZ = maxDistance * sin(angle);
        float startY = (ray % 3 - 1) * 10.0f; // Vary height

        // Calculate impact parameter
        float impactParam = sqrt(startX * startX + startZ * startZ);

        // Skip rays that would be captured (too close to photon sphere)
        if (impactParam < schwarzschildRadius * 2.0f) continue;

        // Calculate deflection angle (Einstein's formula)
        float deflectionAngle = 4.0f * schwarzschildRadius / impactParam;

        // Create curved light ray path
        for (int seg = 0; seg <= segmentsPerRay; ++seg) {
            float t = float(seg) / segmentsPerRay;

            // Distance from black hole center
            float currentX = startX * (1.0f - t);
            float currentZ = startZ * (1.0f - t);
            float distanceFromBH = sqrt(currentX * currentX + currentZ * currentZ);

            // Apply gravitational deflection (stronger near black hole)
            float deflectionFactor = schwarzschildRadius / (distanceFromBH + schwarzschildRadius);
            float bendAngle = deflectionAngle * deflectionFactor * t;

            // Rotate the ray path
            float cosB = cos(bendAngle);
            float sinB = sin(bendAngle);
            float newX = currentX * cosB - currentZ * sinB;
            float newZ = currentX * sinB + currentZ * cosB;

            // Calculate redshift based on distance from black hole
            float redshiftFactor = 1.0f / sqrt(1.0f - schwarzschildRadius / (distanceFromBH + 1.0f));

            // Color based on redshift (blue far away, red close to black hole)
            float r = 0.5f + 0.5f * (1.0f - redshiftFactor);
            float g = 0.8f;
            float b = 0.5f + 0.5f * redshiftFactor;

            // Brightness based on distance
            float brightness = 0.3f + 0.7f * exp(-distanceFromBH * 0.02f);
            r *= brightness;
            g *= brightness;
            b *= brightness;

            vertices.insert(vertices.end(), {newX, startY, newZ, r, g, b});

            if (seg > 0) {
                int currentIndex = ray * (segmentsPerRay + 1) + seg;
                indices.insert(indices.end(), {
                    static_cast<unsigned int>(currentIndex - 1),
                    static_cast<unsigned int>(currentIndex)
                });
            }
        }
    }

    if (!vertices.empty()) {
        glBindVertexArray(g_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

        // Enable blending for glowing light rays
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glLineWidth(3.0f);
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        // Reset blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
    }
}

void renderEinsteinRing(float blackHoleX, float blackHoleY, float blackHoleZ, float radius) {
    // Render Einstein ring effect for perfectly aligned light sources
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const int segments = 64;
    static float time = 0.0f;
    time += 0.01f;

    // Create a glowing ring
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * M_PI / segments;
        float x = blackHoleX + radius * cos(angle);
        float z = blackHoleZ + radius * sin(angle);
        float y = blackHoleY;

        // Pulsating glow effect
        float pulse = 0.7f + 0.3f * sin(time * 3.0f);
        float r = 1.0f * pulse;
        float g = 0.9f * pulse;
        float b = 0.6f * pulse;

        vertices.insert(vertices.end(), {x, y, z, r, g, b});

        if (i > 0) {
            indices.insert(indices.end(), {
                static_cast<unsigned int>(i - 1),
                static_cast<unsigned int>(i)
            });
        }
    }

    // Close the ring
    if (segments > 0) {
        indices.insert(indices.end(), {
            static_cast<unsigned int>(segments),
            static_cast<unsigned int>(0)
        });
    }

    glBindVertexArray(g_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    // Enable additive blending for bright ring
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glLineWidth(4.0f);
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

            indices.insert(indices.end(), {static_cast<unsigned int>(curr), static_cast<unsigned int>(next), static_cast<unsigned int>(curr + 1)});
            indices.insert(indices.end(), {static_cast<unsigned int>(next), static_cast<unsigned int>(next + 1), static_cast<unsigned int>(curr + 1)});
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

// Duplicate function removed - using the first definition

// Duplicate renderStarField function removed - using the first definition

// Keyboard input handler for interactive controls
void handleKeyboardInput(GLFWwindow* window) {
    // Toggle controls with debouncing
    static bool keyPressed[256] = {false};

    // Grid toggle (G key)
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !keyPressed['G']) {
        g_showGrid = !g_showGrid;
        keyPressed['G'] = true;
        std::cout << "Grid: " << (g_showGrid ? "ON" : "OFF") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) {
        keyPressed['G'] = false;
    }

    // Accretion disk toggle (A key)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !keyPressed['A']) {
        g_showAccretionDisk = !g_showAccretionDisk;
        keyPressed['A'] = true;
        std::cout << "Accretion Disk: " << (g_showAccretionDisk ? "ON" : "OFF") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        keyPressed['A'] = false;
    }

    // Particles toggle (P key)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !keyPressed['P']) {
        g_showParticles = !g_showParticles;
        keyPressed['P'] = true;
        std::cout << "Particles: " << (g_showParticles ? "ON" : "OFF") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
        keyPressed['P'] = false;
    }

    // Lensing toggle (L key)
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !keyPressed['L']) {
        g_showLensing = !g_showLensing;
        keyPressed['L'] = true;
        std::cout << "Gravitational Lensing: " << (g_showLensing ? "ON" : "OFF") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
        keyPressed['L'] = false;
    }

    // Black hole toggle (B key)
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !keyPressed['B']) {
        g_showBlackHole = !g_showBlackHole;
        keyPressed['B'] = true;
        std::cout << "Black Hole: " << (g_showBlackHole ? "ON" : "OFF") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        keyPressed['B'] = false;
    }

    // Pause toggle (SPACE key)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !keyPressed[' ']) {
        g_paused = !g_paused;
        keyPressed[' '] = true;
        std::cout << "Animation: " << (g_paused ? "PAUSED" : "PLAYING") << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        keyPressed[' '] = false;
    }

    // Help (H key)
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !keyPressed['H']) {
        keyPressed['H'] = true;
        std::cout << "\n=== BLACK HOLE SIMULATION CONTROLS ===" << std::endl;
        std::cout << "G - Toggle spacetime grid" << std::endl;
        std::cout << "A - Toggle accretion disk" << std::endl;
        std::cout << "P - Toggle particles" << std::endl;
        std::cout << "L - Toggle gravitational lensing" << std::endl;
        std::cout << "B - Toggle black hole visibility" << std::endl;
        std::cout << "SPACE - Pause/Resume animation" << std::endl;
        std::cout << "H - Show this help" << std::endl;
        std::cout << "ESC - Exit simulation" << std::endl;
        std::cout << "======================================\n" << std::endl;
    } else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
        keyPressed['H'] = false;
    }

    // Exit (ESC key)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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

        // Handle keyboard input for interactive controls
        handleKeyboardInput(g_window);

        // Update camera position for orbital movement
        static float time = 0.0f;
        if (!g_paused) {
            time += 0.01f * g_animationSpeed;
        }

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
        GLint timeLoc = glGetUniformLocation(g_shaderProgram, "time");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
        glUniform1f(timeLoc, time);

        // 1. Render background starfield first
        if (g_showStars) {
            glUniform1f(alphaLoc, 0.8f);
            renderStarField();
        }

        // 2. Render spacetime curvature grid (enhanced)
        if (g_showGrid) {
            glUniform1f(alphaLoc, 0.7f);
            renderSpacetimeGrid(120.0f, 24);
        }

        // 3. Render enhanced accretion disk
        if (g_showAccretionDisk) {
            glUniform1f(alphaLoc, 0.9f);
            renderAccretionDisk(8.0f, 35.0f); // Larger, more visible disk
        }

        // 4. Render the black hole event horizon with glowing edge
        if (g_showBlackHole) {
            glUniform1f(alphaLoc, 1.0f);
            renderBlackHoleEventHorizon(0.0f, 0.0f, 0.0f, 6.0f); // Proper Schwarzschild radius
        }

        // 5. Render enhanced orbiting particles
        if (g_showParticles) {
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
            if (g_showTrajectories) {
                glUniform1f(alphaLoc, 0.6f);
                renderParticleTrajectory(x1, 1.0f, z1, x1 + 8.0f, 1.0f, z1 + 8.0f, 0.0f, 1.0f, 0.2f);
                renderParticleTrajectory(x2, 2.0f, z2, x2 + 12.0f, 2.0f, z2 + 12.0f, 1.0f, 0.5f, 0.0f);
                renderParticleTrajectory(x3, -3.0f, z3, x3 + 18.0f, -3.0f, z3 + 18.0f, 0.2f, 0.8f, 1.0f);
            }
        }

        // 7. Render gravitational lensing effects
        if (g_showLensing) {
            glUniform1f(alphaLoc, 0.9f);
            renderGravitationalLensing(0.0f, 0.0f, 0.0f, 6.0f);
        }

        // 8. Render Einstein ring for dramatic effect
        if (g_showEinsteinRing) {
            glUniform1f(alphaLoc, 0.7f);
            renderEinsteinRing(0.0f, 0.0f, 0.0f, 12.0f);
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

#ifdef VISUAL_DEMO_ONLY
// Simplified main function for visual demo
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  BLACK HOLE VISUAL DEMO - ENHANCED" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Initialize Windows OpenGL
    if (!initializeWindowsOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    // Initialize the simple renderer
    if (!initializeSimpleRenderer()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        cleanupWindowsOpenGL();
        return -1;
    }

    std::cout << "Enhanced Black Hole Simulation initialized!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  G - Toggle spacetime grid" << std::endl;
    std::cout << "  A - Toggle accretion disk" << std::endl;
    std::cout << "  P - Toggle particles" << std::endl;
    std::cout << "  L - Toggle gravitational lensing" << std::endl;
    std::cout << "  B - Toggle black hole visibility" << std::endl;
    std::cout << "  SPACE - Pause/Resume animation" << std::endl;
    std::cout << "  H - Show help" << std::endl;
    std::cout << "  ESC - Exit simulation" << std::endl;
    std::cout << std::endl;

    // Main rendering loop
    while (!windowShouldClose()) {
        pollEvents();
        renderSimpleBlackHole();
    }

    // Cleanup
    cleanupSimpleRenderer();
    cleanupWindowsOpenGL();

    std::cout << "Enhanced Black Hole Simulation ended." << std::endl;
    return 0;
}
#endif
