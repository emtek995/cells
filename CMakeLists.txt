cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_COMPILER /home/eddie/.local/bin/clang)
set(CMAKE_CXX_COMPILER /home/eddie/.local/bin/clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(game CXX)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(SDL)

include_directories(src/include)

add_executable(game
  main.cpp
  src/render.cpp)
target_link_libraries(game PRIVATE SDL3::SDL3)
