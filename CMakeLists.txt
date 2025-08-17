cmake_minimum_required(VERSION 3.16)
project(BlackHoleSimulation VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
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
include_directories(${OPENGL_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/PhysicsEngine.cpp
    src/RenderingEngine.cpp
    src/InputSystem.cpp
    src/SimulationManager.cpp
    src/BlackHole.cpp
    src/Particle.cpp
    src/LightRay.cpp
    src/AccretionDisk.cpp
    src/glad.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    gdi32
    user32
    kernel32
    opengl32
)

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