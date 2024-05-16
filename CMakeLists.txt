cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 11)

# Find Raylib package
find_package(raylib 4.5 REQUIRED)

add_executable(Snake src/main.cpp)

target_link_libraries(Snake PRIVATE raylib)

# Add custom command to run the program after building
add_custom_command(TARGET Snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running Snake..."
    COMMAND $<TARGET_FILE:Snake>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
