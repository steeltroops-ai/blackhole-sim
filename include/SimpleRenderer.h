#ifndef SIMPLE_RENDERER_H
#define SIMPLE_RENDERER_H

// Forward declaration for GLFW
struct GLFWwindow;

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the simple 3D renderer
bool initializeSimpleRenderer();

// Render the black hole simulation
void renderSimpleBlackHole();

// Check if the window should close
bool shouldCloseWindow();

// Get the GLFW window handle
GLFWwindow* getSimpleRendererWindow();

// Cleanup the renderer
void cleanupSimpleRenderer();

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_RENDERER_H
