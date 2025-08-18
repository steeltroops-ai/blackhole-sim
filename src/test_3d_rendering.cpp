#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

// Forward declarations from SimpleRenderingEngine.cpp
bool initializeSimpleRenderer();
void renderSimpleBlackHole();
bool shouldCloseWindow();
void cleanupSimpleRenderer();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    Black Hole 3D Rendering Test" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Initialize the simple renderer
    if (!initializeSimpleRenderer()) {
        std::cerr << "Failed to initialize 3D renderer!" << std::endl;
        return -1;
    }
    
    std::cout << "3D Renderer initialized successfully!" << std::endl;
    std::cout << "You should see a 3D black hole simulation window." << std::endl;
    std::cout << "Press ESC or close the window to exit." << std::endl;
    std::cout << std::endl;
    
    // Main rendering loop
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    while (!shouldCloseWindow()) {
        // Render the 3D scene
        renderSimpleBlackHole();
        
        frameCount++;
        
        // Print FPS every 60 frames
        if (frameCount % 60 == 0) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
            double fps = 60000.0 / duration.count();
            std::cout << "FPS: " << fps << " | Frames: " << frameCount << std::endl;
            startTime = currentTime;
        }
        
        // Small delay to prevent excessive CPU usage
        // std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    std::cout << std::endl;
    std::cout << "Cleaning up 3D renderer..." << std::endl;
    cleanupSimpleRenderer();
    
    std::cout << "3D rendering test completed successfully!" << std::endl;
    return 0;
}
