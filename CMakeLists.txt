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

target_include_directories(Pong PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(Pong PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/pdcurses.a")
