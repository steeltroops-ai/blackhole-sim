cmake_minimum_required(VERSION 3.16)
project(BlackHoleSimulation VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find required packages
find_package(OpenGL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/include)
include_directories(${OPENGL_INCLUDE_DIRS})

# Add GLFW library directory
link_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib-static-ucrt)

# Source files
set(SOURCES
    src/main.cpp
    src/PhysicsEngine.cpp
    src/RenderingEngine.cpp
    src/SimpleRenderingEngine.cpp
    src/InputSystem.cpp
    src/SimulationManager.cpp
    src/BlackHole.cpp
    src/Particle.cpp
    src/LightRay.cpp
    src/AccretionDisk.cpp
    src/glad.c
)

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Note: test_3d_rendering.cpp was removed during cleanup

# Link libraries for main executable
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw3
    gdi32
    user32
    kernel32
    opengl32
    shell32
    ole32
    uuid
    winmm
)

# Note: test_3d_rendering target removed during cleanup

# Link OpenMP if available
# if(OpenMP_CXX_FOUND)
#     target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
# endif()

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY docs/ DESTINATION share/doc/${PROJECT_NAME})

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "OpenMP found: ${OpenMP_CXX_FOUND}")