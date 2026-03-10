cmake_minimum_required(VERSION 3.5)
project(STREAM LANGUAGES C)

set(CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PATH}/bin)

add_executable(stream  src/stream.c)

