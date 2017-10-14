cmake_minimum_required(VERSION 3.0)
project(dltrace)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wall)

file(GLOB DIR
        "./*.cpp"
        "./*.hpp"
        "./event/*.cpp"
        "./event/*.hpp"
        "./utils/*.cpp"
        "./utils/*.hpp"
        )

set(SOURCE_FILES ${DIR})
add_executable(dltrace ${SOURCE_FILES})

target_link_libraries(dltrace elf)
