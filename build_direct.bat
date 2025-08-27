@echo off
echo ========================================
echo  DIRECT COMPILATION - BYPASS CMAKE
echo ========================================

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Copy GLFW DLL to build directory
copy "glfw-3.4.bin.WIN64\lib-mingw-w64\glfw3.dll" "build\" >nul 2>&1

echo Compiling with direct g++ approach...

REM Direct compilation with all necessary flags
g++ -std=c++17 ^
    -O3 ^
    -DNDEBUG ^
    -I"include" ^
    -I"glfw-3.4.bin.WIN64/include" ^
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
    src/glad.c ^
    -L"glfw-3.4.bin.WIN64/lib-mingw-w64" ^
    -lglfw3 ^
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
    -o build/BlackHoleSimulation.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Executable created: build\BlackHoleSimulation.exe
    echo.
    echo Testing the enhanced visualization...
    echo.
    echo Controls:
    echo   G - Toggle spacetime grid
    echo   A - Toggle accretion disk
    echo   P - Toggle particles
    echo   L - Toggle gravitational lensing
    echo   B - Toggle black hole visibility
    echo   SPACE - Pause/Resume
    echo   H - Help
    echo   ESC - Exit
    echo.
    echo ========================================
    echo  🚀 LAUNCHING SIMULATION...
    echo ========================================
    echo.
    cd build
    BlackHoleSimulation.exe
    cd ..
) else (
    echo.
    echo ========================================
    echo  ❌ COMPILATION FAILED
    echo ========================================
    echo.
    echo Trying alternative linking approach...
    echo.
    
    REM Try with dynamic library
    g++ -std=c++17 ^
        -O3 ^
        -DNDEBUG ^
        -I"include" ^
        -I"glfw-3.4.bin.WIN64/include" ^
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
        src/glad.c ^
        -L"glfw-3.4.bin.WIN64/lib-mingw-w64" ^
        -lglfw3dll ^
        -lopengl32 ^
        -lgdi32 ^
        -luser32 ^
        -lkernel32 ^
        -lshell32 ^
        -lole32 ^
        -luuid ^
        -lwinmm ^
        -o build/BlackHoleSimulation.exe
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo  ✅ DYNAMIC LINKING SUCCESSFUL!
        echo ========================================
        echo.
        echo Launching enhanced black hole simulation...
        cd build
        BlackHoleSimulation.exe
        cd ..
    ) else (
        echo.
        echo ========================================
        echo  ❌ BOTH APPROACHES FAILED
        echo ========================================
        echo.
        echo Please check:
        echo 1. MinGW is properly installed
        echo 2. GLFW libraries are compatible with your MinGW version
        echo 3. All source files are present
        echo.
    )
)

pause
