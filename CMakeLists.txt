cmake_minimum_required(VERSION 3.5)

project(dataStructure C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
# -fsanitize=address
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CPP_SOURCE "${PROJECT_SOURCE_DIR}/application/src/*.cpp")

find_package(spdlog)
find_package(fmt)
find_package(GTest)

include_directories(
        ${PROJECT_SOURCE_DIR}/application/include
)

add_executable(${PROJECT_NAME} ${CPP_SOURCE})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    spdlog::spdlog 
    fmt::fmt
    gtest::gtest
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/application/include)