cmake_minimum_required(VERSION 3.31)
project(FUTPax)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(FUTPaxLib)

add_executable(FUTPaxExeutable main.cpp)

# copy the images folder and level folder to cmake build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cards/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cards/)

# Copy the formations folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/formations/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/formations/)

# Copy the backgrounds folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/backgrounds/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/backgrounds/)

# Copy the images folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images/)

target_link_libraries(FUTPaxExeutable PRIVATE FUTPaxLib)
