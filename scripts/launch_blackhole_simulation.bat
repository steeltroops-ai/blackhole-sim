@echo off
title Black Hole Simulation - 3D Launcher
color 0A

echo.
echo ========================================
echo     BLACK HOLE SIMULATION LAUNCHER
echo ========================================
echo.
echo Welcome to the Black Hole Simulation!
echo This will launch a stunning 3D visualization of black hole physics
echo including gravitational lensing, particle trajectories, and more.
echo.

REM Check if build directory exists (updated for scripts directory)
if not exist "..\build" (
    echo ERROR: Build directory not found!
    echo Please run the build script first.
    echo.
    pause
    exit /b 1
)

REM Check if executable exists (updated for scripts directory)
if not exist "..\build\BlackHoleSimulation.exe" (
    echo ERROR: BlackHoleSimulation.exe not found!
    echo Please build the project first using: scripts\build_windows.bat
    echo.
    pause
    exit /b 1
)

echo Checking system requirements...
echo - OpenGL 3.3+ support: Required
echo - Graphics drivers: Please ensure they are up to date
echo.

echo Starting 3D Black Hole Simulation...
echo.
echo Controls:
echo - WASD: Move camera
echo - Mouse: Look around
echo - Space: Pause/Resume simulation
echo - R: Reset simulation
echo - G: Toggle grid
echo - P: Toggle particles
echo - L: Toggle light rays
echo - A: Toggle accretion disk
echo - ESC or close window to exit
echo.

REM Change to build directory and run (updated for scripts directory)
cd ..\build
echo Launching full 3D Black Hole Simulation...
BlackHoleSimulation.exe

REM Check exit code
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Simulation completed successfully!
) else (
    echo.
    echo ERROR: Simulation failed to run properly.
    echo Error code: %ERRORLEVEL%
    echo.
    echo Troubleshooting:
    echo 1. Update your graphics drivers
    echo 2. Ensure OpenGL 3.3+ support
    echo 3. Close other graphics-intensive applications
)

echo.
pause
