cmake_minimum_required(VERSION 3.5)

project(dataStructure LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") 
endif()

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
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/application/include)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}_test PRIVATE 
    gtest::gtest
)

target_include_directories(${PROJECT_NAME}_test PRIVATE ${PROJECT_SOURCE_DIR}/application/include)

add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)