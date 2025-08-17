#include "RenderingEngine.hpp"
#include "glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#endif

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
      m_VAO(0), m_VBO(0), m_EBO(0), m_windowWidth(1200), m_windowHeight(800) {
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
    
    m_cameraPosition[0] = 0.0f; m_cameraPosition[1] = 0.0f; m_cameraPosition[2] = 5.0f;
    m_cameraTarget[0] = 0.0f; m_cameraTarget[1] = 0.0f; m_cameraTarget[2] = 0.0f;
    m_cameraUp[0] = 0.0f; m_cameraUp[1] = 1.0f; m_cameraUp[2] = 0.0f;
    
    m_fieldOfView = 45.0f;
    m_nearPlane = 0.1f;
    m_farPlane = 1000.0f;
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
    wc.lpszClassName = L"BlackHoleSimWindow";
    
    if (!RegisterClassEx(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return false;
    }
    
    // Create window
    std::wstring wtitle(title.begin(), title.end());
    m_hwnd = CreateWindowEx(
        0,
        L"BlackHoleSimWindow",
        wtitle.c_str(),
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
    SetMatrix4("model", m_modelMatrix);
    SetMatrix4("view", m_viewMatrix);
    SetMatrix4("projection", m_projectionMatrix);
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
    m_cameraPosition[0] = x;
    m_cameraPosition[1] = y;
    m_cameraPosition[2] = z;
}

void RenderingEngine::SetCameraTarget(float x, float y, float z) {
    m_cameraTarget[0] = x;
    m_cameraTarget[1] = y;
    m_cameraTarget[2] = z;
}

void RenderingEngine::SetCameraUp(float x, float y, float z) {
    m_cameraUp[0] = x;
    m_cameraUp[1] = y;
    m_cameraUp[2] = z;
}

void RenderingEngine::SetFieldOfView(float fov) {
    m_fieldOfView = fov;
    UpdateProjectionMatrix();
}

void RenderingEngine::SetNearFarPlanes(float nearPlane, float farPlane) {
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
        // Position, Color, TexCoord
        x1, y1, z1, r, g, b, 0.0f, 0.0f,
        x2, y2, z2, r, g, b, 1.0f, 1.0f
    };
    
    std::vector<unsigned int> indices = {0, 1};
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    SetFloat("alpha", alpha);
    SetBool("useTexture", false);
    
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void RenderingEngine::RenderPoint(float x, float y, float z, float size, float r, float g, float b, float alpha) {
    if (!m_initialized) return;
    
    glPointSize(size);
    
    std::vector<float> vertices = {
        // Position, Color, TexCoord
        x, y, z, r, g, b, 0.0f, 0.0f
    };
    
    std::vector<unsigned int> indices = {0};
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
    
    SetFloat("alpha", alpha);
    SetBool("useTexture", false);
    
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

void RenderingEngine::CreateBuffers() {
    // Generate vertex array object and buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    // Set vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
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
    SetBool("useTexture", false);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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

void RenderingEngine::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Handle additional window messages if needed
    // This can be extended for input handling, etc.
}
#endif