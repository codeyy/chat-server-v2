cmake_minimum_required(VERSION 3.14)
project(runner LANGUAGES CXX)

# Define the application target. To change its name, change BINARY_NAME in the top-level CMakeLists.txt.
add_executable(${BINARY_NAME} WIN32
  "flutter_window.cpp"
  "main.cpp"
  "utils.cpp"
  "win32_window.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "Runner.rc"
  "runner.exe.manifest"
)

# Apply standard compile flags.
apply_standard_settings(${BINARY_NAME})

# Add preprocessor definitions for the target.
target_compile_definitions(${BINARY_NAME} PRIVATE
  $<$<CONFIG:Debug>:_DEBUG>
  UNICODE
  _UNICODE
)

# Add dependencies.
target_link_libraries(${BINARY_NAME} PRIVATE flutter flutter_wrapper_app)
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
