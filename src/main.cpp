/**
 * @file main.cpp
 * @brief Entry point for the Black Hole Simulation
 * 
 * This file contains the main function and application initialization logic.
 * It sets up the simulation manager and runs the main simulation loop.
 */

#include "SimulationManager.hpp"
#include <iostream>
#include <exception>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

using namespace BlackHoleSim;

/**
 * @brief Print application banner and information
 */
void PrintBanner() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "    Black Hole Simulation v1.0\n";
    std::cout << "========================================\n";
    std::cout << "\n";
    std::cout << "A real-time simulation of black hole physics\n";
    std::cout << "including gravitational lensing, geodesics,\n";
    std::cout << "and accretion disk dynamics.\n";
    std::cout << "\n";
    std::cout << "Controls:\n";
    std::cout << "  WASD     - Move camera\n";
    std::cout << "  Mouse    - Look around\n";
    std::cout << "  Space    - Pause/Resume\n";
    std::cout << "  R        - Reset simulation\n";
    std::cout << "  G        - Toggle grid\n";
    std::cout << "  P        - Toggle particles\n";
    std::cout << "  L        - Toggle light rays\n";
    std::cout << "  A        - Toggle accretion disk\n";
    std::cout << "  H        - Show/Hide help\n";
    std::cout << "  ESC      - Exit\n";
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "\n";
}

/**
 * @brief Print usage information
 * @param programName Name of the program executable
 */
void PrintUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help           Show this help message\n";
    std::cout << "  -v, --version        Show version information\n";
    std::cout << "  -w, --width <width>  Set window width (default: 1280)\n";
    std::cout << "  -h, --height <height> Set window height (default: 720)\n";
    std::cout << "  --fullscreen         Start in fullscreen mode\n";
    std::cout << "  --vsync              Enable vertical sync\n";
    std::cout << "  --no-vsync           Disable vertical sync\n";
    std::cout << "  --msaa <samples>     Set MSAA samples (0, 2, 4, 8, 16)\n";
    std::cout << "  --config <file>      Load configuration from file\n";
    std::cout << "\n";
}

/**
 * @brief Print version information
 */
void PrintVersion() {
    std::cout << "Black Hole Simulation v1.0.0\n";
    std::cout << "Built with C++17, OpenGL 4.3+\n";
    std::cout << "Copyright (c) 2024\n";
}

/**
 * @brief Parse command line arguments
 * @param argc Argument count
 * @param argv Argument values
 * @param config Configuration structure to fill
 * @return True if parsing successful, false if should exit
 */
bool ParseArguments(int argc, char* argv[], SimulationConfig& config) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            PrintUsage(argv[0]);
            return false;
        }
        else if (arg == "-v" || arg == "--version") {
            PrintVersion();
            return false;
        }
        else if (arg == "-w" || arg == "--width") {
            if (i + 1 < argc) {
                config.windowWidth = std::atoi(argv[++i]);
                if (config.windowWidth <= 0) {
                    std::cerr << "Error: Invalid window width: " << argv[i] << "\n";
                    return false;
                }
            } else {
                std::cerr << "Error: --width requires a value\n";
                return false;
            }
        }
        else if (arg == "--height") {
            if (i + 1 < argc) {
                config.windowHeight = std::atoi(argv[++i]);
                if (config.windowHeight <= 0) {
                    std::cerr << "Error: Invalid window height: " << argv[i] << "\n";
                    return false;
                }
            } else {
                std::cerr << "Error: --height requires a value\n";
                return false;
            }
        }
        else if (arg == "--fullscreen") {
            config.fullscreen = true;
        }
        else if (arg == "--vsync") {
            config.vsync = true;
        }
        else if (arg == "--no-vsync") {
            config.vsync = false;
        }
        else if (arg == "--msaa") {
            if (i + 1 < argc) {
                config.msaaSamples = std::atoi(argv[++i]);
                if (config.msaaSamples != 0 && config.msaaSamples != 2 && 
                    config.msaaSamples != 4 && config.msaaSamples != 8 && 
                    config.msaaSamples != 16) {
                    std::cerr << "Error: Invalid MSAA samples: " << argv[i] << "\n";
                    std::cerr << "Valid values: 0, 2, 4, 8, 16\n";
                    return false;
                }
            } else {
                std::cerr << "Error: --msaa requires a value\n";
                return false;
            }
        }
        else if (arg == "--config") {
            if (i + 1 < argc) {
                config.configFile = argv[++i];
            } else {
                std::cerr << "Error: --config requires a file path\n";
                return false;
            }
        }
        else {
            std::cerr << "Error: Unknown argument: " << arg << "\n";
            std::cerr << "Use --help for usage information\n";
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Setup console for Windows (enable UTF-8 and color output)
 */
void SetupConsole() {
#ifdef _WIN32
    // Enable UTF-8 output on Windows
    SetConsoleOutputCP(CP_UTF8);
    
    // Enable ANSI color codes on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            // Define ENABLE_VIRTUAL_TERMINAL_PROCESSING if not available
            #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
            #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
            #endif
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

/**
 * @brief Main application entry point
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit code (0 for success)
 */
int main(int argc, char* argv[]) {
    // Setup console for better output
    SetupConsole();
    
    try {
        // Print application banner
        PrintBanner();
        
        // Create default configuration
        SimulationConfig config;
        
        // Parse command line arguments
        if (!ParseArguments(argc, argv, config)) {
            return 0; // Help or version was shown, exit normally
        }
        
        // Create and initialize simulation manager
        std::cout << "Initializing simulation...\n";
        SimulationManager simulation;
        
        if (!simulation.Initialize(config)) {
            std::cerr << "Error: Failed to initialize simulation\n";
            return 1;
        }
        
        std::cout << "Simulation initialized successfully\n";
        std::cout << "Starting main loop...\n\n";
        
        // Run the main simulation loop
        simulation.Run();
        
        // Cleanup
        std::cout << "\nShutting down simulation...\n";
        simulation.Shutdown();
        
        std::cout << "Simulation terminated successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\nFatal error: " << e.what() << "\n";
        std::cerr << "The simulation will now exit\n";
        return 1;
        
    } catch (...) {
        std::cerr << "\nUnknown fatal error occurred\n";
        std::cerr << "The simulation will now exit\n";
        return 1;
    }
}

/**
 * @brief Application cleanup function (called on exit)
 */
void ApplicationCleanup() {
    // This function can be registered with std::atexit() if needed
    // for additional cleanup beyond what's done in main()
    std::cout << "Application cleanup complete\n";
}