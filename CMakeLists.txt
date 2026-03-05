cmake_minimum_required(VERSION 3.10)
project(SumoBot)

# Create executable with source files
add_executable(main main.cpp src/SumoBot.cpp)

# Add include/ directory to search path
target_include_directories(main PRIVATE include)