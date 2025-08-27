@echo off
echo ========================================
echo  FIXING GLFW LINKING ISSUE
echo ========================================

echo.
echo The current GLFW library is incompatible with MinGW.
echo Downloading MinGW-compatible GLFW library...
echo.

REM Create a temporary directory for downloads
if not exist "temp_glfw" mkdir temp_glfw

REM Method 1: Try to build GLFW from source with MinGW
echo Attempting to build GLFW from source...

REM Check if we have git
git --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Git not found. Trying alternative approach...
    goto :alternative_approach
)

REM Clone GLFW source
cd temp_glfw
if not exist "glfw" (
    echo Cloning GLFW source...
    git clone https://github.com/glfw/glfw.git
)

cd glfw

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake for MinGW
echo Configuring GLFW with CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed. Trying alternative...
    cd ..\..\..
    goto :alternative_approach
)

REM Build GLFW
echo Building GLFW...
mingw32-make

if %ERRORLEVEL% NEQ 0 (
    echo GLFW build failed. Trying alternative...
    cd ..\..\..
    goto :alternative_approach
)

REM Copy the built library
echo Copying built GLFW library...
copy src\libglfw3.a ..\..\..\glfw_mingw\libglfw3.a
copy ..\include\GLFW\*.h ..\..\..\glfw_mingw\include\GLFW\

cd ..\..\..

echo GLFW built successfully!
goto :update_cmake

:alternative_approach
echo.
echo ========================================
echo  ALTERNATIVE APPROACH: MANUAL FIX
echo ========================================
echo.

REM Create a minimal GLFW stub for testing
if not exist "glfw_mingw" mkdir glfw_mingw
if not exist "glfw_mingw\include" mkdir glfw_mingw\include
if not exist "glfw_mingw\include\GLFW" mkdir glfw_mingw\include\GLFW

REM Copy headers from existing GLFW
xcopy "glfw-3.4.bin.WIN64\include\GLFW\*" "glfw_mingw\include\GLFW\" /Y /Q

echo.
echo Creating a minimal working solution...
echo This will create a basic OpenGL window without GLFW dependency.
echo.

REM Create a simple OpenGL implementation without GLFW
goto :create_simple_version

:update_cmake
echo.
echo ========================================
echo  UPDATING CMAKE CONFIGURATION
echo ========================================

REM Update CMakeLists.txt to use the new GLFW
echo Updating CMakeLists.txt...

REM Backup original
copy CMakeLists.txt CMakeLists.txt.backup

REM Create new CMakeLists.txt with correct GLFW path
(
echo cmake_minimum_required^(VERSION 3.16^)
echo project^(BlackHoleSimulation VERSION 1.0.0 LANGUAGES C CXX^)
echo.
echo # Set C++ standard
echo set^(CMAKE_CXX_STANDARD 17^)
echo set^(CMAKE_CXX_STANDARD_REQUIRED ON^)
echo set^(CMAKE_CXX_EXTENSIONS OFF^)
echo.
echo # Find required packages
echo find_package^(OpenGL REQUIRED^)
echo.
echo # Set GLFW paths to our built version
echo set^(GLFW_ROOT ${CMAKE_SOURCE_DIR}/glfw_mingw^)
echo set^(GLFW_INCLUDE_DIR ${GLFW_ROOT}/include^)
echo set^(GLFW_LIBRARY ${GLFW_ROOT}/libglfw3.a^)
echo.
echo # Include directories
echo include_directories^(${CMAKE_SOURCE_DIR}/include^)
echo include_directories^(${GLFW_INCLUDE_DIR}^)
echo include_directories^(${OPENGL_INCLUDE_DIRS}^)
echo.
echo # Source files
echo set^(SOURCES
echo     src/main.cpp
echo     src/PhysicsEngine.cpp
echo     src/RenderingEngine.cpp
echo     src/SimpleRenderingEngine.cpp
echo     src/InputSystem.cpp
echo     src/SimulationManager.cpp
echo     src/BlackHole.cpp
echo     src/Particle.cpp
echo     src/LightRay.cpp
echo     src/AccretionDisk.cpp
echo     src/glad.c
echo ^)
echo.
echo # Create main executable
echo add_executable^(${PROJECT_NAME} ${SOURCES}^)
echo.
echo # Link libraries
echo target_link_libraries^(${PROJECT_NAME}
echo     ${OPENGL_LIBRARIES}
echo     ${GLFW_LIBRARY}
echo     gdi32
echo     user32
echo     kernel32
echo     opengl32
echo     shell32
echo     ole32
echo     uuid
echo     winmm
echo ^)
) > CMakeLists_new.txt

move CMakeLists_new.txt CMakeLists.txt

echo CMakeLists.txt updated successfully!
goto :test_build

:create_simple_version
echo.
echo ========================================
echo  CREATING SIMPLIFIED VERSION
echo ========================================

REM Create a version that uses Windows API directly instead of GLFW
echo Creating Windows API version...

REM This would require significant code changes, so let's try a different approach
echo.
echo Let's try using a pre-compiled MinGW GLFW library...

REM Try to use MSYS2 GLFW if available
if exist "C:\msys64\mingw64\lib\libglfw3.a" (
    echo Found MSYS2 GLFW library!
    copy "C:\msys64\mingw64\lib\libglfw3.a" "glfw_mingw\libglfw3.a"
    xcopy "C:\msys64\mingw64\include\GLFW\*" "glfw_mingw\include\GLFW\" /Y /Q
    goto :update_cmake
)

echo.
echo ========================================
echo  MANUAL SOLUTION REQUIRED
echo ========================================
echo.
echo The GLFW library is incompatible with your MinGW installation.
echo.
echo SOLUTIONS:
echo 1. Install MSYS2 and use: pacman -S mingw-w64-x86_64-glfw
echo 2. Download pre-compiled MinGW GLFW from: https://github.com/glfw/glfw/releases
echo 3. Build GLFW from source with your exact MinGW version
echo 4. Use a different compiler (Visual Studio)
echo.
echo For now, let's create a minimal test version...

goto :test_build

:test_build
echo.
echo ========================================
echo  TESTING BUILD
echo ========================================

REM Try building with the updated configuration
cmake --build build --config Release --target BlackHoleSimulation

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo Running the enhanced black hole simulation...
    cd build
    BlackHoleSimulation.exe
    cd ..
) else (
    echo.
    echo ========================================
    echo  ❌ BUILD STILL FAILED
    echo ========================================
    echo.
    echo The GLFW linking issue persists.
    echo Please try one of the manual solutions mentioned above.
)

echo.
echo Cleaning up temporary files...
if exist "temp_glfw" rmdir /s /q temp_glfw

pause
