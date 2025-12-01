cmake_minimum_required(VERSION 3.29)
project(LSDES)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC} ${INCLUDES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
