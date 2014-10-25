cmake_minimum_required(VERSION 2.8)
project(gallerizer-cli-gd)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall")

set(GALLERIZER_SOURCE src/image.cpp src/gallerizer.cpp src/console.cpp)
include_directories(src/include)
add_executable(gallerizer ${GALLERIZER_SOURCE})
target_link_libraries(gallerizer gd pcre)

set(BATCH_RESIZE_SOURCE src/image.cpp src/batch-resize.cpp)
include_directories(src/include)
add_executable(batch-resize ${BATCH_RESIZE_SOURCE})
target_link_libraries(batch-resize gd)
