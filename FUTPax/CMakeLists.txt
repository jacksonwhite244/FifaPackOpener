cmake_minimum_required(VERSION 3.31)
project(FUTPax)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(FUTPaxLib)

add_executable(FUTPaxExeutable main.cpp)

# copy the imges folder and level folder to cmake build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cards/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cards/)

target_link_libraries(FUTPaxExeutable PRIVATE FUTPaxLib)
