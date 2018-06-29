cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)

project(block_storage)

set(SOURCES
    src/main.cpp
)

add_executable(block_storage ${SOURCES})

target_include_directories(block_storage
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
