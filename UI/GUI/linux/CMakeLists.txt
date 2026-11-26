cmake_minimum_required(VERSION 3.10)
project(runner LANGUAGES CXX)

# Set the application title
set(BINARY_NAME "nexusforge_gui")
set(APPLICATION_ID "com.nexusforge.gui")

cmake_policy(SET CMP0079 NEW)

# Configure build options.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE
    STRING "Configuration: Debug, Profile, Release"
    FORCE)
endif()

# Installation paths
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

# Root directory of Flutter project.
set(FLUTTER_MANAGED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/flutter")

# Flutter configuration.
include(${FLUTTER_MANAGED_DIR}/generated_config.cmake OPTIONAL)

# Application build.
add_executable(${BINARY_NAME}
  "main.cc"
  "my_application.cc"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
)

# Apply the standard set of build settings.
apply_standard_settings(${BINARY_NAME})

# Add dependency libraries and include directories.
target_link_libraries(${BINARY_NAME} PRIVATE flutter)
target_link_libraries(${BINARY_NAME} PRIVATE PkgConfig::GTK)
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
