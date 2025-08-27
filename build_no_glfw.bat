@echo off
echo ========================================
echo  BUILDING WITHOUT GLFW DEPENDENCY
echo ========================================

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

echo.
echo Compiling with Windows OpenGL API (no GLFW)...
echo.

REM Compile with Windows OpenGL implementation
g++ -std=c++17 ^
    -O3 ^
    -DNDEBUG ^
    -I"include" ^
    src/main.cpp ^
    src/PhysicsEngine.cpp ^
    src/RenderingEngine.cpp ^
    src/SimpleRenderingEngine.cpp ^
    src/InputSystem.cpp ^
    src/SimulationManager.cpp ^
    src/BlackHole.cpp ^
    src/Particle.cpp ^
    src/LightRay.cpp ^
    src/AccretionDisk.cpp ^
    src/WindowsOpenGL.cpp ^
    src/glad.c ^
    -lopengl32 ^
    -lgdi32 ^
    -luser32 ^
    -lkernel32 ^
    -lshell32 ^
    -lole32 ^
    -luuid ^
    -lwinmm ^
    -static-libgcc ^
    -static-libstdc++ ^
    -o build/BlackHoleSimulation_NoGLFW.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Enhanced Black Hole Simulation built successfully!
    echo Executable: build\BlackHoleSimulation_NoGLFW.exe
    echo.
    echo Features:
    echo ✅ Event horizon with glowing neon edge
    echo ✅ Physics-based accretion disk colors
    echo ✅ Curved spacetime grid visualization
    echo ✅ Interactive particle system
    echo ✅ Gravitational lensing effects
    echo ✅ Anime-style shader enhancements
    echo.
    echo Interactive Controls:
    echo   G - Toggle spacetime grid
    echo   A - Toggle accretion disk
    echo   P - Toggle particles
    echo   L - Toggle gravitational lensing
    echo   B - Toggle black hole visibility
    echo   SPACE - Pause/Resume animation
    echo   H - Show help
    echo   ESC - Exit simulation
    echo.
    echo ========================================
    echo  🚀 LAUNCHING ENHANCED SIMULATION...
    echo ========================================
    echo.
    cd build
    BlackHoleSimulation_NoGLFW.exe
    cd ..
) else (
    echo.
    echo ========================================
    echo  ❌ COMPILATION FAILED
    echo ========================================
    echo.
    echo Checking for missing dependencies...
    
    REM Check if OpenGL32.lib exists
    if not exist "C:\Windows\System32\opengl32.dll" (
        echo ❌ OpenGL not found. Please install graphics drivers.
    ) else (
        echo ✅ OpenGL found.
    )
    
    REM Check if MinGW is properly installed
    g++ --version >nul 2>&1
    if %ERRORLEVEL% NEQ 0 (
        echo ❌ MinGW not found. Please install MinGW-w64.
    ) else (
        echo ✅ MinGW found.
    )
    
    echo.
    echo Please check the error messages above and ensure:
    echo 1. MinGW-w64 is properly installed
    echo 2. Graphics drivers are installed
    echo 3. All source files are present
    echo.
    echo If the issue persists, try:
    echo 1. Update graphics drivers
    echo 2. Reinstall MinGW-w64
    echo 3. Check Windows SDK installation
)

echo.
echo Build process completed.
pause
