# CMake generated Testfile for 
# Source directory: C:/Omniverse/Projects/blackhole-sim/tests
# Build directory: C:/Omniverse/Projects/blackhole-sim/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(PhysicsTests "C:/Omniverse/Projects/blackhole-sim/build/tests/physics_test.exe")
set_tests_properties(PhysicsTests PROPERTIES  TIMEOUT "60" WORKING_DIRECTORY "C:/Omniverse/Projects/blackhole-sim/build" _BACKTRACE_TRIPLES "C:/Omniverse/Projects/blackhole-sim/tests/CMakeLists.txt;25;add_test;C:/Omniverse/Projects/blackhole-sim/tests/CMakeLists.txt;0;")
add_test(MinimalTest "C:/Omniverse/Projects/blackhole-sim/build/tests/minimal_test.exe")
set_tests_properties(MinimalTest PROPERTIES  TIMEOUT "30" WORKING_DIRECTORY "C:/Omniverse/Projects/blackhole-sim/build" _BACKTRACE_TRIPLES "C:/Omniverse/Projects/blackhole-sim/tests/CMakeLists.txt;54;add_test;C:/Omniverse/Projects/blackhole-sim/tests/CMakeLists.txt;0;")
