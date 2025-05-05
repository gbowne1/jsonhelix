cmake_minimum_required(VERSION 3.10)
project(JsonHelix LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS -Wall -Wextra -Werror -pedantic -O0 -g)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Define source files
set(SOURCES
    src/main.c
    src/x11_window.c
    src/renderer.c
    src/input.c
    src/json_parser.c
    src/json_validator.c
    src/json_tools.c
    src/api_client.c
    src/bson.c
    src/settings.c
    src/utils.c
)

# Define header files
set(HEADERS
    include/json_parser.h
    include/json_validator.h
    include/json_tools.h
    include/api_client.h
    include/bson.h
    include/settings.h
    include/utils.h
)

# Create the executable
add_executable(JsonHelix ${SOURCES} ${HEADERS})

# Link X11 libraries
find_package(X11 REQUIRED)
target_link_libraries(JsonHelix PRIVATE X11::X11)

# Include directories
target_include_directories(JsonHelix PRIVATE include)

# Set CMake properties for Windows
if(WIN32)
    set_target_properties(JsonHelix PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# Add custom build commands
add_custom_target(run ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building JsonHelix..."
    COMMENT "Running custom build steps"
)
add_dependencies(JsonHelix run)