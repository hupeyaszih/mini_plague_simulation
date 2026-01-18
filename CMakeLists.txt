cmake_minimum_required(VERSION 3.16)

project(plague_simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(plague_simulation
    src/main.cpp
)

target_include_directories(plague_simulation
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
