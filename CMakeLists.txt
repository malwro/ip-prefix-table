# Basic config
cmake_minimum_required(VERSION 3.0.0)
project(Phoenix VERSION 0.1.0)

# Test config
# enable_testing()
# include (CTest)
# find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
# INCLUDE(Dart)

# Libraries
add_library(Lib prefix_table.c)
add_library(Gen generator.c)

# Executables
add_executable(Main main.c)
# add_test(test Main)

# Linking
target_link_libraries(Main PUBLIC Lib)
target_link_libraries(Main PUBLIC Gen)

# Target properties
set_target_properties(Main PROPERTIES LINKER_LANGUAGE C)
set_target_properties(Lib PROPERTIES LINKER_LANGUAGE C)
set_target_properties(Gen PROPERTIES LINKER_LANGUAGE C)
