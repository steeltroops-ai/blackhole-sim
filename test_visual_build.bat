@echo off
echo Testing Direct GLFW Compilation...

REM Try direct compilation with g++
g++ -std=c++17 ^
    -I"include" ^
    -I"glfw-3.4.bin.WIN64/include" ^
    -L"glfw-3.4.bin.WIN64/lib-mingw-w64" ^
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
    -lglfw3 ^
    -lopengl32 ^
    -lgdi32 ^
    -luser32 ^
    -lkernel32 ^
    -lshell32 ^
    -lole32 ^
    -luuid ^
    -lwinmm ^
    -o build/BlackHoleSimulation_direct.exe

if %ERRORLEVEL% EQU 0 (
    echo SUCCESS: Direct compilation worked!
    echo Running enhanced visual simulation...
    cd build
    BlackHoleSimulation_direct.exe
) else (
    echo FAILED: Direct compilation failed
    echo Trying alternative approach...
)

pause
