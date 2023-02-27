# Basic config
cmake_minimum_required(VERSION 3.0.0)
project(Phoenix VERSION 0.1.0)

# Test config
# enable_testing()
# include (CTest)
# find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
# INCLUDE(Dart)

# Text files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/random_ips.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/net_addresses.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/masks.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_ips.txt
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Libraries
add_library(Lib prefix_storage.c)
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
