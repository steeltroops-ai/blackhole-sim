@echo off
echo Building BlackHole Simulation Project (Full Version)...

set MINGW_PATH=C:\MinGW\bin
set COMPILER=%MINGW_PATH%\mingw32-g++.exe
set FLAGS=-std=c++17 -Wall -Wextra -O2
set INCLUDE_DIR=-I.\include -I.\glfw-3.4.bin.WIN64\include
set SRC_DIR=.\src
set TEST_DIR=.\tests
set BUILD_DIR=.\build
set GLFW_LIB=.\glfw-3.4.bin.WIN64\lib-mingw-w64
set LIBS=-L%GLFW_LIB% -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32

echo.
echo === Creating Build Directory ===
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
echo ✓ Build directory ready

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
echo === Building Core Modules ===
echo Compiling BlackHole...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\BlackHole.cpp -o %BUILD_DIR%\BlackHole.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile BlackHole
    exit /b 1
)

echo Compiling Particle...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\Particle.cpp -o %BUILD_DIR%\Particle.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile Particle
    exit /b 1
)

echo Compiling LightRay...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\LightRay.cpp -o %BUILD_DIR%\LightRay.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile LightRay
    exit /b 1
)

echo Compiling AccretionDisk...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\AccretionDisk.cpp -o %BUILD_DIR%\AccretionDisk.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile AccretionDisk
    exit /b 1
)

echo Compiling PhysicsEngine...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\PhysicsEngine.cpp -o %BUILD_DIR%\PhysicsEngine.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile PhysicsEngine
    exit /b 1
)

echo Compiling RenderingEngine...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\RenderingEngine.cpp -o %BUILD_DIR%\RenderingEngine.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile RenderingEngine
    exit /b 1
)

echo Compiling InputSystem...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\InputSystem.cpp -o %BUILD_DIR%\InputSystem.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile InputSystem
    exit /b 1
)

echo Compiling SimulationManager...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\SimulationManager.cpp -o %BUILD_DIR%\SimulationManager.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile SimulationManager
    exit /b 1
)

echo Compiling GLAD...
%COMPILER% %FLAGS% %INCLUDE_DIR% -c %SRC_DIR%\glad.c -o %BUILD_DIR%\glad.o
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to compile GLAD
    exit /b 1
)

echo ✓ All modules compiled successfully

echo.
echo === Linking Final Executable ===
%COMPILER% %FLAGS% %INCLUDE_DIR% %SRC_DIR%\main.cpp %BUILD_DIR%\*.o %LIBS% -o %BUILD_DIR%\blackhole_sim.exe
if %ERRORLEVEL% neq 0 (
    echo ERROR: Failed to link final executable
    exit /b 1
)

echo.
echo === Copying Required DLLs ===
copy "%GLFW_LIB%\glfw3.dll" "%BUILD_DIR%\" >nul
if %ERRORLEVEL% neq 0 (
    echo WARNING: Could not copy glfw3.dll
) else (
    echo ✓ GLFW DLL copied
)

echo.
echo === Build Summary ===
echo ✓ All tests passed
echo ✓ All modules compiled
echo ✓ Final executable created: %BUILD_DIR%\blackhole_sim.exe
echo ✓ Required DLLs copied
echo.
echo Build completed successfully!
echo.
echo To run the simulation:
echo   cd %BUILD_DIR%
echo   blackhole_sim.exe
echo.
pause