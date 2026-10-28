# 1. Enforce a modern CMake minimum version
cmake_minimum_required(VERSION 3.20...3.30)

# 2. Define the project name and metadata
project(AresNexus
    VERSION 1.0.0
    LANGUAGES CXX
)

# 6. Define the main executable target
add_executable(chat-server core-server/main.cpp)

