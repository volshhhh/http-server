cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    headers/
    headers/entity
    headers/response_builders
)

file(GLOB SOURCES
    "src/*.cpp"
    "src/response_builders/*.cpp"
    "main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})