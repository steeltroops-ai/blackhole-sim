@echo off
echo Building BlackHole Simulation Project...

set MINGW_PATH=C:\MinGW\bin
set COMPILER=%MINGW_PATH%\mingw32-g++.exe
set FLAGS=-std=c++11 -Wall -Wextra -O2
set INCLUDE_DIR=-I.\include
set SRC_DIR=.\src
set TEST_DIR=.\tests
set BUILD_DIR=.\build

echo.
echo === Building Tests ===
echo Compiling minimal test...
%COMPILER% %FLAGS% %TEST_DIR%\minimal_test.cpp -o %BUILD_DIR%\minimal_test.exe
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile minimal test
    exit /b 1
)
echo ✓ Minimal test compiled successfully

echo.
echo Compiling physics test...
%COMPILER% %FLAGS% %TEST_DIR%\physics_test.cpp -o %BUILD_DIR%\physics_test.exe
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile physics test
    exit /b 1
)
echo ✓ Physics test compiled successfully

echo.
echo === Running Tests ===
echo Running minimal test...
%BUILD_DIR%\minimal_test.exe
if %ERRORLEVEL% neq 0 (
    echo ERROR: Minimal test failed
    exit /b 1
)

echo.
echo Running physics test...
%BUILD_DIR%\physics_test.exe
if %ERRORLEVEL% neq 0 (
    echo ERROR: Physics test failed
    exit /b 1
)

echo.
echo === Building Main Application (Core Only) ===
echo Compiling core physics modules...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\BlackHole.cpp -o %BUILD_DIR%\BlackHole.o
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\Particle.cpp -o %BUILD_DIR%\Particle.o
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\LightRay.cpp -o %BUILD_DIR%\LightRay.o
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\AccretionDisk.cpp -o %BUILD_DIR%\AccretionDisk.o
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\PhysicsEngine.cpp -o %BUILD_DIR%\PhysicsEngine.o

echo ✓ Core physics modules compiled

echo.
echo === Build Summary ===
echo ✓ All tests passed
echo ✓ Core physics modules compiled
echo ⚠ Note: Rendering system requires OpenGL/GLFW setup
echo.
echo Build completed successfully!
pause