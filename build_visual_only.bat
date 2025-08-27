@echo off
echo ========================================
echo  BUILDING VISUAL COMPONENTS ONLY
echo ========================================

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

echo.
echo Compiling only the enhanced visual components...
echo This will create a working demonstration of the visual improvements.
echo.

REM Compile only the essential visual components without complex dependencies
g++ -std=c++17 ^
    -O3 ^
    -DNDEBUG ^
    -I"include" ^
    src/SimpleRenderingEngine.cpp ^
    src/WindowsOpenGL.cpp ^
    src/glad.c ^
    -DVISUAL_DEMO_ONLY ^
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
    -o build/BlackHoleVisualDemo.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  ✅ VISUAL DEMO COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Enhanced Black Hole Visual Demo built successfully!
    echo Executable: build\BlackHoleVisualDemo.exe
    echo.
    echo This demo showcases all the visual enhancements:
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
    echo  🚀 LAUNCHING VISUAL DEMO...
    echo ========================================
    echo.
    cd build
    BlackHoleVisualDemo.exe
    cd ..
) else (
    echo.
    echo ========================================
    echo  ❌ VISUAL DEMO COMPILATION FAILED
    echo ========================================
    echo.
    echo Trying alternative minimal approach...
    
    REM Try with just the core rendering functions
    g++ -std=c++17 ^
        -O3 ^
        -DNDEBUG ^
        -DMINIMAL_DEMO ^
        src/WindowsOpenGL.cpp ^
        src/glad.c ^
        -lopengl32 ^
        -lgdi32 ^
        -luser32 ^
        -lkernel32 ^
        -o build/MinimalOpenGLDemo.exe
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo  ✅ MINIMAL DEMO SUCCESSFUL!
        echo ========================================
        echo.
        echo Basic OpenGL window created successfully.
        echo This proves the Windows API approach works.
        echo.
        cd build
        MinimalOpenGLDemo.exe
        cd ..
    ) else (
        echo.
        echo ========================================
        echo  ❌ ALL APPROACHES FAILED
        echo ========================================
        echo.
        echo Please check:
        echo 1. MinGW is properly installed and in PATH
        echo 2. Graphics drivers are installed
        echo 3. OpenGL is available on your system
        echo.
        echo Error details above show the specific issues.
    )
)

echo.
echo Build process completed.
pause
