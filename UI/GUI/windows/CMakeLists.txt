cmake_minimum_required(VERSION 3.14)
project(nexusforge_gui LANGUAGES CXX)

set(BINARY_NAME "nexusforge_gui")

cmake_policy(SET CMP0079 NEW)

# Configure build options.
get_property(IS_MULTICONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTICONFIG)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Profile;Release"
    CACHE STRING "" FORCE)
else()
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE
      STRING "Configuration: Debug, Profile, Release"
      FORCE)
  endif()
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

# Root directory of Flutter project.
set(FLUTTER_MANAGED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/flutter")

# Flutter configuration.
include(${FLUTTER_MANAGED_DIR}/generated_config.cmake OPTIONAL)

# Application build.
add_subdirectory("runner")

# Generated plugin build.
add_subdirectory("${FLUTTER_MANAGED_DIR}" "flutter_ephemeral")
