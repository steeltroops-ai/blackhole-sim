# CMakeLists.txt for Black Hole Simulation Tests
cmake_minimum_required(VERSION 3.16)

# Include directories (updated for new structure)
include_directories(${CMAKE_SOURCE_DIR}/include/blackhole_sim)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Create simple physics test executable (updated path)
add_executable(physics_test
    unit/physics_test.cpp
)

# Set C++ standard
target_compile_features(physics_test PRIVATE cxx_std_11)

# Add compiler flags for better debugging and warnings
if(MSVC)
    target_compile_options(physics_test PRIVATE /W4)
else()
    target_compile_options(physics_test PRIVATE -Wall -Wextra)
endif()

# Add test to CTest
enable_testing()
add_test(NAME PhysicsTests COMMAND physics_test)

# Set test properties
set_tests_properties(PhysicsTests PROPERTIES
    TIMEOUT 60  # 1 minute timeout
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add custom target to run tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS physics_test
    COMMENT "Running physics tests"
)

# Create minimal test executable (updated path)
add_executable(minimal_test
    unit/minimal_test.cpp
)

target_compile_features(minimal_test PRIVATE cxx_std_11)

if(MSVC)
    target_compile_options(minimal_test PRIVATE /W4)
else()
    target_compile_options(minimal_test PRIVATE -Wall -Wextra)
endif()

# Add minimal test
add_test(NAME MinimalTest COMMAND minimal_test)
set_tests_properties(MinimalTest PROPERTIES
    TIMEOUT 30
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Create integration test executable
add_executable(integration_test
    integration/test_physics.cpp
)

target_compile_features(integration_test PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(integration_test PRIVATE /W4)
else()
    target_compile_options(integration_test PRIVATE -Wall -Wextra)
endif()

# Add integration test
add_test(NAME IntegrationTest COMMAND integration_test)
set_tests_properties(IntegrationTest PROPERTIES
    TIMEOUT 60
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Create benchmark test executable
add_executable(benchmark_test
    benchmarks/test_physics_performance.cpp
)

target_compile_features(benchmark_test PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(benchmark_test PRIVATE /W4)
else()
    target_compile_options(benchmark_test PRIVATE -Wall -Wextra)
endif()

# Add benchmark test
add_test(NAME BenchmarkTest COMMAND benchmark_test)
set_tests_properties(BenchmarkTest PROPERTIES
    TIMEOUT 120
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)