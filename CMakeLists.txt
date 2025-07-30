cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.6
)

FetchContent_MakeAvailable(magic_enum)

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

target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)