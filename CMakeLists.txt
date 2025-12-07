cmake_minimum_required(VERSION 4.0)
project(Pong)

set(CMAKE_CXX_STANDARD 23)

# Add executable
add_executable(Pong
        src/Ponger.cpp
        src/main.cpp
        src/Ball.cpp
        src/TextureManager.cpp
)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

target_include_directories(Pong PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Pong PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/pdcurses.a")
