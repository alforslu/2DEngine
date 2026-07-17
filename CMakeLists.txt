cmake_minimum_required(VERSION 4.0)
# Not tried on lower than 4.3.3

project(2DECS)

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
# git clone https://github.com/libsdl-org/SDL.git vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(2DECS src/main.cpp src/registry.cpp src/systems/render.cpp src/systems/update_movement.cpp)

target_include_directories(2DECS PRIVATE include)

# Link to the actual SDL3 library.
target_link_libraries(2DECS PRIVATE SDL3::SDL3)
