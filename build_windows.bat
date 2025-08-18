@echo off
title Black Hole Simulation - Build Script
color 0B

echo.
echo ========================================
echo     BLACK HOLE SIMULATION BUILD
echo ========================================
echo.

REM Configuration
set BUILD_DIR=build
set BUILD_TYPE=Release

REM Parse command line arguments
:parse_args
if "%1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if "%1"=="--clean" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo ✓ Build directory cleaned
    shift
    goto parse_args
)
if "%1"=="--help" (
    echo Usage: %0 [OPTIONS]
    echo Options:
    echo   --debug    Build in debug mode
    echo   --clean    Clean build directory first
    echo   --help     Show this help message
    exit /b 0
)
if not "%1"=="" (
    echo Unknown option: %1
    echo Use --help for usage information
    exit /b 1
)

REM Check for CMake
cmake --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake not found. Please install CMake.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

REM Create build directory
echo Creating build directory...
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
echo ✓ Build directory ready

REM Configure with CMake
echo.
echo Configuring with CMake...
cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)
echo ✓ CMake configuration successful

REM Build with CMake
echo.
echo Building with CMake...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)
echo ✓ Build successful

REM Check if executable was created
if exist "%BUILD_DIR%\BlackHoleSimulation.exe" (
    echo.
    echo ========================================
    echo     BUILD COMPLETED SUCCESSFULLY!
    echo ========================================
    echo.
    echo Executable: %BUILD_DIR%\BlackHoleSimulation.exe
    echo To run: cd %BUILD_DIR% ^&^& BlackHoleSimulation.exe
    echo Or use: launch_blackhole_simulation.bat
    echo.
) else (
    echo ERROR: Executable was not created
    pause
    exit /b 1
)

echo Build process finished.
pause
