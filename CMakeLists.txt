cmake_minimum_required(VERSION 3.10)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/HttpServer.cpp
    src/ResponseBuilder.cpp
    src/Request.cpp
    src/ResponseFactory.cpp
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE headers)