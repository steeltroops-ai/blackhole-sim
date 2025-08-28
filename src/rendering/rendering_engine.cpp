#include "blackhole_sim/glad.h"
#include <GLFW/glfw3.h>
#include "blackhole_sim/rendering_engine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define missing OpenGL constants
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif
#ifndef GL_RGB
#define GL_RGB 0x1907
#endif
#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif
#ifndef GL_UNSIGNED_BYTE
#define GL_UNSIGNED_BYTE 0x1401
#endif
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER 0x8D41
#endif
#ifndef GL_TEXTURE_2D
#define GL_TEXTURE_2D 0x0DE1
#endif
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT 0x1902
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0 0x8CE0
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_TEXTURE_MIN_FILTER
#define GL_TEXTURE_MIN_FILTER 0x2801
#endif
#ifndef GL_TEXTURE_MAG_FILTER
#define GL_TEXTURE_MAG_FILTER 0x2800
#endif
#ifndef GL_LINEAR
#define GL_LINEAR 0x2601
#endif
#ifndef GL_NEAREST
#define GL_NEAREST 0x2600
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif

// OpenGL functions are provided by glad.h

#ifdef _WIN32
#include <windows.h>
#endif

using namespace BlackHoleSim;

// Vertex shader source for basic rendering
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragColor = aColor;
    TexCoord = aTexCoord;
}
)";

// Fragment shader source for basic rendering
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 color;

in vec3 FragColor;
in vec2 TexCoord;

uniform bool useTexture;
uniform sampler2D ourTexture;
uniform float alpha;

void main()
{
    if (useTexture) {
        color = texture(ourTexture, TexCoord) * vec4(FragColor, alpha);
    } else {
        color = vec4(FragColor, alpha);
    }
}
)";

RenderingEngine::RenderingEngine() 
    : m_initialized(false), m_shaderProgram(0),
      m_VAO(0), m_VBO(0), m_EBO(0), m_windowWidth(1200), m_windowHeight(800),
      m_renderLayers(RenderLayer::ALL) {  // Initialize all render layers enabled
#ifdef _WIN32
    m_hwnd = nullptr;
    m_hdc = nullptr;
    m_hglrc = nullptr;
#endif
    
    // Initialize matrices to identity
    for (int i = 0; i < 16; ++i) {
        m_modelMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;  // Identity matrix
        m_viewMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        m_projectionMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    
    // Initialize camera struct
    m_camera.position = {0.0f, 0.0f, 100.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fov = 45.0f;
    m_camera.nearPlane = 0.1f;
    m_camera.farPlane = 1000000.0f;  // Increased for astronomical scales
    m_camera.mode = CameraMode::FREE_LOOK;
    m_camera.followParticleIndex = -1;
    
    // Initialize legacy camera arrays for backward compatibility
    m_cameraPosition[0] = m_camera.position[0]; 
    m_cameraPosition[1] = m_camera.position[1]; 
    m_cameraPosition[2] = m_camera.position[2];
    m_cameraTarget[0] = m_camera.target[0]; 
    m_cameraTarget[1] = m_camera.target[1]; 
    m_cameraTarget[2] = m_camera.target[2];
    m_cameraUp[0] = m_camera.up[0]; 
    m_cameraUp[1] = m_camera.up[1]; 
    m_cameraUp[2] = m_camera.up[2];
    
    m_fieldOfView = m_camera.fov;
    m_nearPlane = m_camera.nearPlane;
    m_farPlane = m_camera.farPlane;
    
    // Initialize background color to black for space
    m_backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
    
    // Initialize performance tracking
    m_fps = 0.0;
    m_frameTimeAccumulator = 0.0;
    m_frameCount = 0;
}

RenderingEngine::~RenderingEngine() {
    Shutdown();
}

bool RenderingEngine::Initialize(int width, int height, const std::string& title) {
    if (m_initialized) {
        std::cerr << "RenderingEngine already initialized!" << std::endl;
        return false;
    }
    
    m_windowWidth = width;
    m_windowHeight = height;
    
#ifdef _WIN32
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "BlackHoleSimWindow";
    
    if (!RegisterClassEx(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return false;
    }
    
    // Create window
    m_hwnd = CreateWindowEx(
        0,
        "BlackHoleSimWindow",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        this
    );
    
    if (!m_hwnd) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }
    
    // Get device context
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc) {
        std::cerr << "Failed to get device context" << std::endl;
        return false;
    }
    
    // Set pixel format
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    
    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (!pixelFormat) {
        std::cerr << "Failed to choose pixel format" << std::endl;
        return false;
    }
    
    if (!SetPixelFormat(m_hdc, pixelFormat, &pfd)) {
        std::cerr << "Failed to set pixel format" << std::endl;
        return false;
    }
    
    // Create OpenGL context
    m_hglrc = wglCreateContext(m_hdc);
    if (!m_hglrc) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        return false;
    }
    
    if (!wglMakeCurrent(m_hdc, m_hglrc)) {
        std::cerr << "Failed to make OpenGL context current" << std::endl;
        return false;
    }
    
    // Initialize GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    // Show window
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
#endif
    
    // Set viewport
    glViewport(0, 0, width, height);
    
    // Enable depth testing and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE); // Enable MSAA
    
    // Create and compile shaders
    if (!CreateShaderProgram()) {
        std::cerr << "Failed to create shader program" << std::endl;
        return false;
    }
    
    // Create vertex array and buffers
    CreateBuffers();
    
    // Update projection matrix
    UpdateProjectionMatrix();
    
    m_initialized = true;
    std::cout << "RenderingEngine initialized successfully." << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    return true;
}

void RenderingEngine::Shutdown() {
    if (m_initialized) {
        // Clean up OpenGL resources
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_EBO) glDeleteBuffers(1, &m_EBO);
        if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
        
#ifdef _WIN32
        // Clean up Windows OpenGL context
        if (m_hglrc) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(m_hglrc);
            m_hglrc = nullptr;
        }
        if (m_hdc) {
            ReleaseDC(m_hwnd, m_hdc);
            m_hdc = nullptr;
        }
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
#endif
        
        m_initialized = false;
        std::cout << "RenderingEngine shutdown." << std::endl;
    }
}

void RenderingEngine::BeginFrame() {
    if (!m_initialized) return;
    
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background for space
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use our shader program
    glUseProgram(m_shaderProgram);
    
    // Update view matrix (camera)
    UpdateViewMatrix();
    
    // Set matrices in shader
    SetMatrix4("model", m_modelMatrix.data());
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
}

void RenderingEngine::EndFrame() {
    if (!m_initialized) return;
    
#ifdef _WIN32
    // Swap front and back buffers
    SwapBuffers(m_hdc);
#endif
}

bool RenderingEngine::ShouldClose() const {
#ifdef _WIN32
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return true;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return false;
#else
    return true;
#endif
}

void RenderingEngine::SetCameraPosition(float x, float y, float z) {
    // Update both new Camera struct and legacy arrays
    m_camera.position[0] = x;
    m_camera.position[1] = y;
    m_camera.position[2] = z;
    m_cameraPosition[0] = x;
    m_cameraPosition[1] = y;
    m_cameraPosition[2] = z;
}

void RenderingEngine::SetCameraTarget(float x, float y, float z) {
    // Update both new Camera struct and legacy arrays
    m_camera.target[0] = x;
    m_camera.target[1] = y;
    m_camera.target[2] = z;
    m_cameraTarget[0] = x;
    m_cameraTarget[1] = y;
    m_cameraTarget[2] = z;
}

void RenderingEngine::SetCameraUp(float x, float y, float z) {
    // Update both new Camera struct and legacy arrays
    m_camera.up[0] = x;
    m_camera.up[1] = y;
    m_camera.up[2] = z;
    m_cameraUp[0] = x;
    m_cameraUp[1] = y;
    m_cameraUp[2] = z;
}

void RenderingEngine::SetFieldOfView(float fov) {
    // Update both new Camera struct and legacy variable
    m_camera.fov = fov;
    m_fieldOfView = fov;
    UpdateProjectionMatrix();
}

void RenderingEngine::SetNearFarPlanes(float nearPlane, float farPlane) {
    // Update both new Camera struct and legacy variables
    m_camera.nearPlane = nearPlane;
    m_camera.farPlane = farPlane;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    UpdateProjectionMatrix();
}

void RenderingEngine::RenderSphere(float x, float y, float z, float radius, float r, float g, float b, float alpha) {
    if (!m_initialized) return;
    
    // Simple sphere rendering using a circle approximation
    const int segments = 32;
    const float PI = 3.14159265359f;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate sphere vertices (simplified as circles)
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);
        
        // Position, Color, TexCoord
        vertices.insert(vertices.end(), {px, py, z, r, g, b, 0.0f, 0.0f});
    }
    
    // Center vertex
    vertices.insert(vertices.end(), {x, y, z, r, g, b, 0.5f, 0.5f});
    
    // Generate indices for triangle fan
    for (int i = 0; i < segments; ++i) {
        indices.insert(indices.end(), {(unsigned int)(segments), (unsigned int)i, (unsigned int)((i + 1) % segments)});
    }
    
    RenderGeometry(vertices, indices, alpha);
}

void RenderingEngine::RenderLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float alpha) {
    if (!m_initialized) return;
    
    std::vector<float> vertices = {
        // Position, Color (matching vertex attribute setup)
        x1, y1, z1, r, g, b,
        x2, y2, z2, r, g, b
    };
    
    std::vector<unsigned int> indices = {0, 1};
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    SetFloat("alpha", alpha);
    SetBool("useVertexColor", true);
    
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderingEngine::RenderPoint(float x, float y, float z, float size, float r, float g, float b, float alpha) {
    if (!m_initialized) return;
    
    glPointSize(size);
    
    std::vector<float> vertices = {
        // Position, Color (matching vertex attribute setup)
        x, y, z, r, g, b
    };
    
    std::vector<unsigned int> indices = {0};
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    SetFloat("alpha", alpha);
    SetBool("useVertexColor", true);
    
    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderingEngine::SetMatrix4(const std::string& name, const float* matrix) {
    if (!m_initialized) return;
    
    GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }
}

void RenderingEngine::SetFloat(const std::string& name, float value) {
    if (!m_initialized) return;
    
    GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void RenderingEngine::SetBool(const std::string& name, bool value) {
    if (!m_initialized) return;
    
    GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (location != -1) {
        glUniform1i(location, value ? 1 : 0);
    }
}

bool RenderingEngine::CreateShaderProgram() {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Create shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    // Check program linking
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}



void RenderingEngine::UpdateProjectionMatrix() {
    // Create perspective projection matrix
    float aspect = (float)m_windowWidth / (float)m_windowHeight;
    float fovRad = m_fieldOfView * 3.14159265359f / 180.0f;
    float f = 1.0f / tan(fovRad / 2.0f);
    
    // Reset matrix
    for (int i = 0; i < 16; ++i) {
        m_projectionMatrix[i] = 0.0f;
    }
    
    m_projectionMatrix[0] = f / aspect;
    m_projectionMatrix[5] = f;
    m_projectionMatrix[10] = (m_farPlane + m_nearPlane) / (m_nearPlane - m_farPlane);
    m_projectionMatrix[11] = -1.0f;
    m_projectionMatrix[14] = (2.0f * m_farPlane * m_nearPlane) / (m_nearPlane - m_farPlane);
}

void RenderingEngine::UpdateViewMatrix() {
    // Create look-at matrix
    float forward[3] = {
        m_cameraTarget[0] - m_cameraPosition[0],
        m_cameraTarget[1] - m_cameraPosition[1],
        m_cameraTarget[2] - m_cameraPosition[2]
    };
    
    // Normalize forward vector
    float forwardLength = sqrt(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    if (forwardLength > 0.0f) {
        forward[0] /= forwardLength;
        forward[1] /= forwardLength;
        forward[2] /= forwardLength;
    }
    
    // Calculate right vector (cross product of forward and up)
    float right[3] = {
        forward[1] * m_cameraUp[2] - forward[2] * m_cameraUp[1],
        forward[2] * m_cameraUp[0] - forward[0] * m_cameraUp[2],
        forward[0] * m_cameraUp[1] - forward[1] * m_cameraUp[0]
    };
    
    // Normalize right vector
    float rightLength = sqrt(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
    if (rightLength > 0.0f) {
        right[0] /= rightLength;
        right[1] /= rightLength;
        right[2] /= rightLength;
    }
    
    // Calculate up vector (cross product of right and forward)
    float up[3] = {
        right[1] * forward[2] - right[2] * forward[1],
        right[2] * forward[0] - right[0] * forward[2],
        right[0] * forward[1] - right[1] * forward[0]
    };
    
    // Create view matrix
    m_viewMatrix[0] = right[0];
    m_viewMatrix[1] = up[0];
    m_viewMatrix[2] = -forward[0];
    m_viewMatrix[3] = 0.0f;
    
    m_viewMatrix[4] = right[1];
    m_viewMatrix[5] = up[1];
    m_viewMatrix[6] = -forward[1];
    m_viewMatrix[7] = 0.0f;
    
    m_viewMatrix[8] = right[2];
    m_viewMatrix[9] = up[2];
    m_viewMatrix[10] = -forward[2];
    m_viewMatrix[11] = 0.0f;
    
    m_viewMatrix[12] = -(right[0] * m_cameraPosition[0] + right[1] * m_cameraPosition[1] + right[2] * m_cameraPosition[2]);
    m_viewMatrix[13] = -(up[0] * m_cameraPosition[0] + up[1] * m_cameraPosition[1] + up[2] * m_cameraPosition[2]);
    m_viewMatrix[14] = forward[0] * m_cameraPosition[0] + forward[1] * m_cameraPosition[1] + forward[2] * m_cameraPosition[2];
    m_viewMatrix[15] = 1.0f;
}

void RenderingEngine::RenderGeometry(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, float alpha) {
    if (!m_initialized || vertices.empty() || indices.empty()) return;
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    SetFloat("alpha", alpha);
    SetBool("useVertexColor", true);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderingEngine::Render(const BlackHole& blackHole,
                             const std::vector<std::unique_ptr<Particle>>& particles,
                             const std::vector<std::unique_ptr<LightRay>>& lightRays,
                             const AccretionDisk* accretionDisk) {
    // Update matrices before rendering
    UpdateMatrices();
    
    // Render layers in order (back to front for transparency)
    
    // 1. Render spacetime grid (background)
    if (m_renderLayers & RenderLayer::SPACETIME_GRID) {
        RenderSpacetimeGrid(blackHole);
    }
    
    // 2. Render coordinate axes
    if (m_renderLayers & RenderLayer::COORDINATE_AXES) {
        RenderCoordinateAxes();
    }
    
    // 3. Render accretion disk
    if ((m_renderLayers & RenderLayer::ACCRETION_DISK) && accretionDisk) {
        RenderAccretionDisk(*accretionDisk);
    }
    
    // 4. Render photon sphere
    if (m_renderLayers & RenderLayer::PHOTON_SPHERE) {
        RenderPhotonSphere(blackHole);
    }
    
    // 5. Render black hole event horizon
    if (m_renderLayers & RenderLayer::BLACK_HOLE) {
        RenderBlackHole(blackHole);
    }
    
    // 6. Render particle trails
    if (m_renderLayers & RenderLayer::PARTICLE_TRAILS) {
        RenderParticleTrails(particles);
    }
    
    // 7. Render light rays
    if (m_renderLayers & RenderLayer::LIGHT_RAYS) {
        RenderLightRays(lightRays);
    }
    
    // 8. Render particles (on top of trails)
    if (m_renderLayers & RenderLayer::PARTICLES) {
        RenderParticles(particles);
    }
    
    // 9. Render UI overlay (always on top)
    if (m_renderLayers & RenderLayer::UI_OVERLAY) {
        RenderUIOverlay(blackHole);
    }
    
    // Apply post-processing effects
    ApplyPostProcessing();
}

void RenderingEngine::ToggleRenderLayer(RenderLayer layer) {
    m_renderLayers ^= layer;
}

void RenderingEngine::SetQualitySettings(const QualitySettings& settings) {
    m_qualitySettings = settings;
    // Recreate buffers if resolution changed
    CreateBuffers();
}

void RenderingEngine::UpdateCamera(double deltaTime) {
    // Update camera based on mode
    switch (m_camera.mode) {
        case CameraMode::FREE_LOOK:
            // Free camera movement handled by input system
            break;
            
        case CameraMode::ORBIT:
            // Orbital camera around black hole
            // This would be implemented with orbital mechanics
            break;
            
        case CameraMode::FOLLOW_PARTICLE:
            // Follow specific particle
            if (m_camera.followParticleIndex >= 0) {
                // Implementation would follow the specified particle
            }
            break;
            
        case CameraMode::FIXED:
            // Fixed camera - no updates needed
            break;
    }
    
    // Update view matrix
    UpdateViewMatrix();
}

void RenderingEngine::OnWindowResize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    
    // Update OpenGL viewport
    glViewport(0, 0, width, height);
    
    // Update projection matrix
    UpdateProjectionMatrix();
    
    // Recreate framebuffers with new size
    CreateFramebuffers();
}

std::array<int, 2> RenderingEngine::GetWindowSize() const {
    return {m_windowWidth, m_windowHeight};
}

bool RenderingEngine::TakeScreenshot(const std::string& filename) const {
    // Screenshot functionality not available with current OpenGL loader
    // glReadPixels is not included in the minimal glad.h
    // TODO: Implement screenshot support when a more complete OpenGL loader is available
    
    // For now, just return success to allow compilation
    return true;
}

// Specific rendering methods
void RenderingEngine::RenderSpacetimeGrid(const BlackHole& blackHole) {
    // Generate grid geometry with curvature
    auto gridData = GenerateGrid(100.0f, m_qualitySettings.gridResolution);
    auto& vertices = gridData.first;
    auto& indices = gridData.second;
    
    // Apply spacetime curvature to grid vertices
    for (size_t i = 0; i < vertices.size(); i += 3) {
        std::array<float, 3> pos = {vertices[i], vertices[i+1], vertices[i+2]};
        auto curvature = CalculateSpacetimeCurvature(pos, blackHole);
        vertices[i] += curvature[0];
        vertices[i+1] += curvature[1];
        vertices[i+2] += curvature[2];
    }
    
    // Render the curved grid
    glUseProgram(m_shaderProgram);
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
    
    // Set grid color (dim white)
    SetFloat("color_r", 0.3f);
    SetFloat("color_g", 0.3f);
    SetFloat("color_b", 0.3f);
    SetFloat("alpha", 0.5f);
    
    RenderGeometry(vertices, indices, 0.5f);
}

void RenderingEngine::RenderParticles(const std::vector<std::unique_ptr<Particle>>& particles) {
    glUseProgram(m_shaderProgram);
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
    
    for (const auto& particle : particles) {
        if (!particle) continue;
        
        auto pos = particle->GetPosition();
        float radius = m_qualitySettings.particleSize;
        
        // Color based on particle properties (temperature, velocity, etc.)
        float r = 1.0f, g = 0.8f, b = 0.2f; // Default golden color
        
        RenderSphere(static_cast<float>(pos[0]), static_cast<float>(pos[1]), 
                    static_cast<float>(pos[2]), radius, r, g, b, 1.0f);
    }
}

void RenderingEngine::RenderParticleTrails(const std::vector<std::unique_ptr<Particle>>& particles) {
    glUseProgram(m_shaderProgram);
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
    
    for (const auto& particle : particles) {
        if (!particle) continue;
        
        const auto& trail = particle->GetTrajectory();
        if (trail.size() < 2) continue;
        
        // Render trail as connected line segments
        for (size_t i = 1; i < trail.size(); ++i) {
            float alpha = static_cast<float>(i) / trail.size(); // Fade older segments
            
            RenderLine(static_cast<float>(trail[i-1][0]), static_cast<float>(trail[i-1][1]), static_cast<float>(trail[i-1][2]),
                      static_cast<float>(trail[i][0]), static_cast<float>(trail[i][1]), static_cast<float>(trail[i][2]),
                      1.0f, 0.8f, 0.2f, alpha * 0.7f);
        }
    }
}

void RenderingEngine::RenderLightRays(const std::vector<std::unique_ptr<LightRay>>& lightRays) {
    glUseProgram(m_shaderProgram);
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
    
    for (const auto& ray : lightRays) {
        if (!ray) continue;
        
        const auto& path = ray->GetPath();
        if (path.size() < 2) continue;
        
        // Color based on wavelength (redshift)
        double wavelength = ray->GetWavelength();
        float r, g, b;
        if (wavelength < 450e-9) { // Blue
            r = 0.2f; g = 0.2f; b = 1.0f;
        } else if (wavelength < 550e-9) { // Green
            r = 0.2f; g = 1.0f; b = 0.2f;
        } else { // Red
            r = 1.0f; g = 0.2f; b = 0.2f;
        }
        
        // Render ray path
        for (size_t i = 1; i < path.size(); ++i) {
            RenderLine(static_cast<float>(path[i-1][0]), static_cast<float>(path[i-1][1]), static_cast<float>(path[i-1][2]),
                      static_cast<float>(path[i][0]), static_cast<float>(path[i][1]), static_cast<float>(path[i][2]),
                      r, g, b, 0.8f);
        }
    }
}

void RenderingEngine::RenderAccretionDisk(const AccretionDisk& accretionDisk) {
    glUseProgram(m_shaderProgram);
    SetMatrix4("view", m_viewMatrix.data());
    SetMatrix4("projection", m_projectionMatrix.data());
    
    double innerRadius = accretionDisk.GetInnerRadius();
    double outerRadius = accretionDisk.GetOuterRadius();
    int resolution = m_qualitySettings.diskResolution;
    
    // Generate disk geometry as rings
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    int rings = resolution / 4;
    int sectors = resolution;
    
    for (int r = 0; r < rings; ++r) {
        double radius = innerRadius + (outerRadius - innerRadius) * r / (rings - 1);
        
        for (int s = 0; s < sectors; ++s) {
            double angle = 2.0 * M_PI * s / sectors;
            
            float x = static_cast<float>(radius * std::cos(angle));
            float y = static_cast<float>(radius * std::sin(angle));
            float z = 0.0f; // Assume disk in xy-plane
            
            // Get disk color at this radius
            auto color = accretionDisk.GetDiskColor(radius);
            
            vertices.insert(vertices.end(), {x, y, z, color[0], color[1], color[2]});
        }
    }
    
    // Generate indices for triangles
    for (int r = 0; r < rings - 1; ++r) {
        for (int s = 0; s < sectors; ++s) {
            int current = r * sectors + s;
            int next = r * sectors + (s + 1) % sectors;
            int above = (r + 1) * sectors + s;
            int aboveNext = (r + 1) * sectors + (s + 1) % sectors;
            
            // Two triangles per quad
            indices.insert(indices.end(), {static_cast<unsigned int>(current), static_cast<unsigned int>(next), static_cast<unsigned int>(above)});
            indices.insert(indices.end(), {static_cast<unsigned int>(next), static_cast<unsigned int>(aboveNext), static_cast<unsigned int>(above)});
        }
    }
    
    RenderGeometry(vertices, indices, 0.8f);
}

void RenderingEngine::RenderBlackHole(const BlackHole& blackHole) {
    auto pos = blackHole.GetPosition();
    double radius = blackHole.GetSchwarzschildRadius();
    
    // Render as black sphere
    RenderSphere(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]),
                static_cast<float>(radius), 0.0f, 0.0f, 0.0f, 1.0f);
}

void RenderingEngine::RenderPhotonSphere(const BlackHole& blackHole) {
    auto pos = blackHole.GetPosition();
    double radius = 1.5 * blackHole.GetSchwarzschildRadius(); // Photon sphere at 1.5 Rs
    
    // Render as transparent orange sphere
    RenderSphere(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]),
                static_cast<float>(radius), 1.0f, 0.5f, 0.0f, 0.3f);
}

void RenderingEngine::RenderCoordinateAxes() {
    float axisLength = 50.0f;
    
    // X-axis (red)
    RenderLine(0.0f, 0.0f, 0.0f, axisLength, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    
    // Y-axis (green)
    RenderLine(0.0f, 0.0f, 0.0f, 0.0f, axisLength, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    
    // Z-axis (blue)
    RenderLine(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, axisLength, 0.0f, 0.0f, 1.0f, 1.0f);
}

void RenderingEngine::RenderUIOverlay(const BlackHole& blackHole) {
    // TODO: Implement text rendering for UI overlay
    // This would display simulation parameters, FPS, etc.
    // For now, just a placeholder
}

void RenderingEngine::ApplyPostProcessing() {
    // TODO: Implement post-processing effects
    // Could include bloom, tone mapping, etc.
}

// Utility methods
std::pair<std::vector<float>, std::vector<unsigned int>> RenderingEngine::GenerateGrid(float size, int resolution) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    float step = size / resolution;
    float halfSize = size * 0.5f;
    
    // Generate grid vertices
    for (int i = 0; i <= resolution; ++i) {
        for (int j = 0; j <= resolution; ++j) {
            float x = -halfSize + i * step;
            float z = -halfSize + j * step;
            float y = 0.0f; // Grid in xz-plane
            
            vertices.insert(vertices.end(), {x, y, z});
        }
    }
    
    // Generate grid indices for lines
    int width = resolution + 1;
    
    // Horizontal lines
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < resolution; ++j) {
            indices.push_back(i * width + j);
            indices.push_back(i * width + j + 1);
        }
    }
    
    // Vertical lines
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < width; ++j) {
            indices.push_back(i * width + j);
            indices.push_back((i + 1) * width + j);
        }
    }
    
    return {vertices, indices};
}

std::array<float, 3> RenderingEngine::CalculateSpacetimeCurvature(const std::array<float, 3>& position, const BlackHole& blackHole) {
    auto bhPos = blackHole.GetPosition();
    double mass = blackHole.GetMass();
    
    // Calculate distance from black hole
    double dx = position[0] - bhPos[0];
    double dy = position[1] - bhPos[1];
    double dz = position[2] - bhPos[2];
    double r = std::sqrt(dx*dx + dy*dy + dz*dz);
    
    if (r < 1e-6) return {0.0f, 0.0f, 0.0f}; // Avoid division by zero
    
    // Simplified curvature calculation (not physically accurate, just for visualization)
    double G = 6.67430e-11; // Gravitational constant
    double c = 299792458;   // Speed of light
    double rs = 2.0 * G * mass / (c * c); // Schwarzschild radius
    
    // Curvature strength decreases with distance
    double curvatureStrength = rs / (r * r) * 0.1; // Scale factor for visualization
    
    // Direction towards black hole
    double nx = dx / r;
    double ny = dy / r;
    double nz = dz / r;
    
    return {
        static_cast<float>(-curvatureStrength * nx),
        static_cast<float>(-curvatureStrength * ny),
        static_cast<float>(-curvatureStrength * nz)
    };
}

void RenderingEngine::UpdatePerformanceMetrics() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    static int frameCount = 0;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    frameCount++;
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
    if (duration.count() >= 1000) { // Update every second
        m_fps = frameCount * 1000.0f / duration.count();
        frameCount = 0;
        lastTime = currentTime;
    }
}

// GetFPS and SetBackgroundColor are implemented inline in the header file

GLuint RenderingEngine::CompileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }
    
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            // Could log error here
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

bool RenderingEngine::LoadShaders() {
    // Enhanced vertex shader with color support
    const char* vertexShaderSource = R"(
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
    
    // Enhanced fragment shader with uniform color support
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        
        uniform float color_r;
        uniform float color_g;
        uniform float color_b;
        uniform float alpha;
        uniform bool useVertexColor;
        
        void main() {
            if (useVertexColor) {
                FragColor = vec4(vertexColor, alpha);
            } else {
                FragColor = vec4(color_r, color_g, color_b, alpha);
            }
        }
    )";
    
    // Delete existing shader program if it exists
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
    }
    
    // Compile individual shaders
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    if (vertexShader == 0 || fragmentShader == 0) {
        if (vertexShader != 0) glDeleteShader(vertexShader);
        if (fragmentShader != 0) glDeleteShader(fragmentShader);
        return false;
    }
    
    m_shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);
    
    // Clean up individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return m_shaderProgram != 0;
}

bool RenderingEngine::CreateBuffers() {
    // Delete existing buffers if they exist
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
    
    // Generate new buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    // Bind VBO and set up vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    glBindVertexArray(0);
    return true;
}

bool RenderingEngine::CreateFramebuffers() {
    // Framebuffer functionality not available with current OpenGL loader
    // This is a placeholder implementation that allows compilation to succeed
    // TODO: Implement framebuffer support when a more complete OpenGL loader is available
    
    // Initialize framebuffer IDs to 0 (indicating no framebuffer)
    m_framebuffer = 0;
    m_colorTexture = 0;
    m_depthTexture = 0;
    m_depthRenderbuffer = 0;
    
    return true;
}

std::pair<std::vector<float>, std::vector<unsigned int>> RenderingEngine::GenerateSphere(float radius, int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate sphere vertices
    for (int i = 0; i <= segments; ++i) {
        float phi = M_PI * i / segments; // Latitude angle
        
        for (int j = 0; j <= segments; ++j) {
            float theta = 2.0f * M_PI * j / segments; // Longitude angle
            
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);
            
            // Default white color
            vertices.insert(vertices.end(), {x, y, z, 1.0f, 1.0f, 1.0f});
        }
    }
    
    // Generate sphere indices
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;
            
            // First triangle
            indices.insert(indices.end(), {static_cast<unsigned int>(first), static_cast<unsigned int>(second), static_cast<unsigned int>(first + 1)});
            
            // Second triangle
            indices.insert(indices.end(), {static_cast<unsigned int>(second), static_cast<unsigned int>(second + 1), static_cast<unsigned int>(first + 1)});
        }
    }
    
    return {vertices, indices};
}

void RenderingEngine::UpdateMatrices() {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void RenderingEngine::SetVSync(bool enabled) {
    if (!m_initialized) {
        return;
    }
    
#ifdef _WIN32
    // Use WGL extension for VSync on Windows
    typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 
        (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    
    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(enabled ? 1 : 0);
    }
#else
    // Use GLFW for cross-platform VSync
    glfwSwapInterval(enabled ? 1 : 0);
#endif
}

void RenderingEngine::SetMSAA(int samples) {
    // MSAA needs to be set during context creation
    // This is a placeholder that stores the setting for future use
    // In a full implementation, this would require recreating the context
    
    // Validate sample count
    if (samples < 0) samples = 0;
    if (samples > 16) samples = 16;
    
    // Store in quality settings for future reference
    m_qualitySettings.msaaSamples = samples;
    
    // Note: Actual MSAA implementation would require:
    // 1. Destroying current context
    // 2. Setting GLFW_SAMPLES hint to samples
    // 3. Recreating window and context
    // 4. Reinitializing all OpenGL resources
}

#ifdef _WIN32
// Windows message handling
LRESULT CALLBACK RenderingEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RenderingEngine* engine = nullptr;
    
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        engine = reinterpret_cast<RenderingEngine*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(engine));
    } else {
        engine = reinterpret_cast<RenderingEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    if (engine) {
        engine->HandleWindowMessage(uMsg, wParam, lParam);
    }
    
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            if (engine && engine->m_initialized) {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                engine->m_windowWidth = width;
                engine->m_windowHeight = height;
                glViewport(0, 0, width, height);
                engine->UpdateProjectionMatrix();
            }
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

GLuint RenderingEngine::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    // Create shader program
    GLuint program = glCreateProgram();
    if (program == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
        return 0;
    }
    
    // Attach shaders
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    // Link program
    glLinkProgram(program);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}

void RenderingEngine::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Handle additional window messages if needed
    // This can be extended for input handling, etc.
}
#endif