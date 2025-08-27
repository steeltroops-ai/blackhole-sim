// Windows OpenGL implementation to replace GLFW
#include <windows.h>
#include <GL/gl.h>
#include <iostream>
#include <string>

// Global variables
static HWND g_hWnd = nullptr;
static HDC g_hDC = nullptr;
static HGLRC g_hRC = nullptr;
static bool g_running = true;
static int g_windowWidth = 1200;
static int g_windowHeight = 800;

// Key state tracking
static bool g_keys[256] = {false};
static bool g_keyPressed[256] = {false};

// Function declarations
extern "C" {
    int initializeWindowsOpenGL();
    void cleanupWindowsOpenGL();
    int windowShouldClose();
    void swapBuffers();
    void pollEvents();
    int getKey(int key);
    void setWindowShouldClose(int value);
}

// Forward declarations for SimpleRenderingEngine functions
extern "C" {
    bool initializeSimpleRenderer();
    void cleanupSimpleRenderer();
    void renderSimpleBlackHole();
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            g_running = false;
            return 0;
            
        case WM_KEYDOWN:
            if (wParam < 256) {
                g_keys[wParam] = true;
                g_keyPressed[wParam] = true;
            }
            return 0;
            
        case WM_KEYUP:
            if (wParam < 256) {
                g_keys[wParam] = false;
            }
            return 0;
            
        case WM_SIZE:
            g_windowWidth = LOWORD(lParam);
            g_windowHeight = HIWORD(lParam);
            glViewport(0, 0, g_windowWidth, g_windowHeight);
            return 0;
            
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

extern "C" int initializeWindowsOpenGL() {
    // Register window class (using ANSI version)
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "BlackHoleSimulation";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return 0;
    }
    
    // Create window (using ANSI version)
    g_hWnd = CreateWindowExA(
        0,
        "BlackHoleSimulation",
        "Black Hole Simulation - Enhanced Anime Style",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        g_windowWidth, g_windowHeight,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (!g_hWnd) {
        std::cerr << "Failed to create window" << std::endl;
        return 0;
    }
    
    // Get device context
    g_hDC = GetDC(g_hWnd);
    if (!g_hDC) {
        std::cerr << "Failed to get device context" << std::endl;
        return 0;
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
    
    int pixelFormat = ChoosePixelFormat(g_hDC, &pfd);
    if (!pixelFormat) {
        std::cerr << "Failed to choose pixel format" << std::endl;
        return 0;
    }
    
    if (!SetPixelFormat(g_hDC, pixelFormat, &pfd)) {
        std::cerr << "Failed to set pixel format" << std::endl;
        return 0;
    }
    
    // Create OpenGL context
    g_hRC = wglCreateContext(g_hDC);
    if (!g_hRC) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        return 0;
    }
    
    if (!wglMakeCurrent(g_hDC, g_hRC)) {
        std::cerr << "Failed to make OpenGL context current" << std::endl;
        return 0;
    }
    
    // Show window
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
    
    // Initialize OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::cout << "Windows OpenGL initialized successfully!" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    return 1;
}

extern "C" void cleanupWindowsOpenGL() {
    if (g_hRC) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(g_hRC);
        g_hRC = nullptr;
    }
    
    if (g_hDC) {
        ReleaseDC(g_hWnd, g_hDC);
        g_hDC = nullptr;
    }
    
    if (g_hWnd) {
        DestroyWindow(g_hWnd);
        g_hWnd = nullptr;
    }
    
    UnregisterClassA("BlackHoleSimulation", GetModuleHandle(nullptr));
}

extern "C" int windowShouldClose() {
    return !g_running;
}

extern "C" void swapBuffers() {
    SwapBuffers(g_hDC);
}

extern "C" void pollEvents() {
    // Reset key pressed states
    for (int i = 0; i < 256; ++i) {
        g_keyPressed[i] = false;
    }
    
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

extern "C" int getKey(int key) {
    if (key < 0 || key >= 256) return 0;
    
    // Map common keys
    switch (key) {
        case 'G': case 'g': return g_keyPressed['G'] || g_keyPressed['g'] ? 1 : 0;
        case 'A': case 'a': return g_keyPressed['A'] || g_keyPressed['a'] ? 1 : 0;
        case 'P': case 'p': return g_keyPressed['P'] || g_keyPressed['p'] ? 1 : 0;
        case 'L': case 'l': return g_keyPressed['L'] || g_keyPressed['l'] ? 1 : 0;
        case 'B': case 'b': return g_keyPressed['B'] || g_keyPressed['b'] ? 1 : 0;
        case 'H': case 'h': return g_keyPressed['H'] || g_keyPressed['h'] ? 1 : 0;
        case ' ': return g_keyPressed[VK_SPACE] ? 1 : 0;
        case 27: return g_keyPressed[VK_ESCAPE] ? 1 : 0; // ESC
        default: return g_keyPressed[key] ? 1 : 0;
    }
}

extern "C" void setWindowShouldClose(int value) {
    g_running = !value;
}

// Additional utility functions
extern "C" void getWindowSize(int* width, int* height) {
    *width = g_windowWidth;
    *height = g_windowHeight;
}

// GLFW API replacement functions
extern "C" int glfwInit(void) {
    return 1; // Always succeed for Windows API
}

extern "C" void glfwTerminate(void) {
    cleanupWindowsOpenGL();
}

extern "C" void* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share) {
    g_windowWidth = width;
    g_windowHeight = height;

    if (initializeWindowsOpenGL()) {
        return (void*)g_hWnd; // Return window handle as GLFWwindow*
    }
    return nullptr;
}

extern "C" void glfwDestroyWindow(void* window) {
    cleanupWindowsOpenGL();
}

extern "C" int glfwWindowShouldClose(void* window) {
    return windowShouldClose();
}

extern "C" void glfwSetWindowShouldClose(void* window, int value) {
    setWindowShouldClose(value);
}

extern "C" void glfwSwapBuffers(void* window) {
    swapBuffers();
}

extern "C" void glfwPollEvents(void) {
    pollEvents();
}

extern "C" int glfwGetKey(void* window, int key) {
    return getKey(key);
}

extern "C" int glfwGetMouseButton(void* window, int button) {
    // Simple mouse button implementation
    if (button == 0) { // Left button
        return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
    } else if (button == 1) { // Right button
        return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
    } else if (button == 2) { // Middle button
        return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 1 : 0;
    }
    return 0;
}

extern "C" void glfwGetCursorPos(void* window, double* xpos, double* ypos) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(g_hWnd, &point);
    *xpos = (double)point.x;
    *ypos = (double)point.y;
}

extern "C" void glfwSetWindowUserPointer(void* window, void* pointer) {
    // Store user pointer (simplified implementation)
    SetWindowLongPtr(g_hWnd, GWLP_USERDATA, (LONG_PTR)pointer);
}

extern "C" void* glfwGetWindowUserPointer(void* window) {
    // Get user pointer
    return (void*)GetWindowLongPtr(g_hWnd, GWLP_USERDATA);
}

extern "C" void glfwGetWindowSize(void* window, int* width, int* height) {
    getWindowSize(width, height);
}

extern "C" void glfwMakeContextCurrent(void* window) {
    // Already handled in initialization
}

extern "C" void glfwWindowHint(int hint, int value) {
    // Window hints are ignored in our Windows API implementation
}

extern "C" void glfwSwapInterval(int interval) {
    // V-sync control - simplified implementation
    // Could be implemented with wglSwapIntervalEXT if needed
}

// Callback storage (simplified - only one callback per type)
static void (*g_keyCallback)(void*, int, int, int, int) = nullptr;
static void (*g_mouseButtonCallback)(void*, int, int, int) = nullptr;
static void (*g_cursorPosCallback)(void*, double, double) = nullptr;
static void (*g_scrollCallback)(void*, double, double) = nullptr;

extern "C" void* glfwSetKeyCallback(void* window, void (*callback)(void*, int, int, int, int)) {
    void* old = (void*)g_keyCallback;
    g_keyCallback = callback;
    return old;
}

extern "C" void* glfwSetMouseButtonCallback(void* window, void (*callback)(void*, int, int, int)) {
    void* old = (void*)g_mouseButtonCallback;
    g_mouseButtonCallback = callback;
    return old;
}

extern "C" void* glfwSetCursorPosCallback(void* window, void (*callback)(void*, double, double)) {
    void* old = (void*)g_cursorPosCallback;
    g_cursorPosCallback = callback;
    return old;
}

extern "C" void* glfwSetScrollCallback(void* window, void (*callback)(void*, double, double)) {
    void* old = (void*)g_scrollCallback;
    g_scrollCallback = callback;
    return old;
}

#ifdef MINIMAL_DEMO
// Minimal demo main function
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  MINIMAL OPENGL DEMO" << std::endl;
    std::cout << "========================================" << std::endl;

    if (!initializeWindowsOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    std::cout << "OpenGL window created successfully!" << std::endl;
    std::cout << "Press ESC to exit..." << std::endl;

    // Simple rendering loop
    while (!windowShouldClose()) {
        pollEvents();

        // Clear screen with a nice color
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        swapBuffers();
    }

    cleanupWindowsOpenGL();
    std::cout << "Demo completed successfully!" << std::endl;
    return 0;
}
#endif
