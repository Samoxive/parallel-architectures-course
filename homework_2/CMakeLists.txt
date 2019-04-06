cmake_minimum_required(VERSION 3.10)
project(homework_2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -g")

add_executable(q1 q1.c)
target_link_libraries(q1 OpenMP::OpenMP_C)
add_executable(q2 q2.c)
target_link_libraries(q2 OpenMP::OpenMP_C)
add_executable(q3 q3.c)
target_link_libraries(q3 OpenMP::OpenMP_C)