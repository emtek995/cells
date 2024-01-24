cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_COMPILER /home/eddie/.local/bin/clang)
set(CMAKE_CXX_COMPILER /home/eddie/.local/bin/clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -stdlib=libc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(GAME LANGUAGES C CXX VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(SDL)

include_directories(src/include)

add_executable(game
  main.cpp
  src/render.cpp
  src/cellmap.cpp)

target_link_libraries(game PRIVATE SDL3::SDL3)
