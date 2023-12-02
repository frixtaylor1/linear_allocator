cmake_minimum_required(VERSION 3.16)
project(LINEAL_ALLOCATOR)

set(CMAKE_CXX_STANDARD 11)


set(
    ENTRYPOINT
    src/main.cpp
)

add_executable(LINEAL_ALLOCATOR 
    ${ENTRYPOINT} 
)