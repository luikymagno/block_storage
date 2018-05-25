cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)

project(BlockStorage)

set(SOURCES
    src/main.cpp
    src/storage_functions.cpp
)

add_executable(BlockStorage ${SOURCES})

target_include_directories(BlockStorage
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
