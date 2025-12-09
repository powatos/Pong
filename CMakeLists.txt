cmake_minimum_required(VERSION 3.5)
project(Pong)

set(CMAKE_CXX_STANDARD 23)

# Add executable
add_executable(Pong
        src/Ponger.cpp
        src/main.cpp
        src/Ball.cpp
        src/TextureManager.cpp
)

# Common warnings
add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
)

# Platform-specific flags
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Windows/Linux
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libc++ -static")
    endif()
else()
    # macOS (x86_64 or ARM64)
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        message(STATUS "Building for Apple Silicon (ARM64)")
    elseif(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
        message(STATUS "Building for Intel macOS")
    endif()

    # Include your ncurses built via OSXCross
    target_include_directories(Pong PRIVATE
            "${CMAKE_SOURCE_DIR}/Dependencies/ncurses/include"
    )
    target_link_directories(Pong PRIVATE
            "${CMAKE_SOURCE_DIR}/Dependencies/ncurses/lib"
    )
    target_link_libraries(Pong PRIVATE ncurses)
endif()

# Always include your headers
target_include_directories(Pong PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Windows static pdcurses
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(Pong PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/pdcurses.a")
endif()
