cmake_minimum_required(VERSION 3.10)
project(sumobot)

# Create executable with source files
add_executable(main_sim sim/main_sim.cpp src/SumoBot.cpp)

# Add include/ directory to search path
target_include_directories(main_sim PRIVATE include)