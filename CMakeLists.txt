cmake_minimum_required(VERSION 3.15)
project(Varcon CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find vulkan package and check if works
find_package(Vulkan REQUIRED)

# Source files
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/headers/*.hpp)

# Path to SDL3 includes and libs
include_directories(
    "${CMAKE_SOURCE_DIR}/includes"
    "${CMAKE_SOURCE_DIR}/headers"
)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# target_compile_options(${PROJECT_NAME} PRIVATE
# $<$<CXX_COMPILER_ID:GNU>:
# -Wall -Wextra -Wpedantic -Werror
#
# -Wconversion -Wshadow -Wfloat-equal -Winvalid-pch
# -Wnon-virtual-dtor
# >
# )

# Link SDL3
target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    SDL3
)
