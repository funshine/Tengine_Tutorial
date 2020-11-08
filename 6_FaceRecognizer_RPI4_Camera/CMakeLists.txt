cmake_minimum_required(VERSION 3.10)
project(face-recognizer-tengine-camera)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)
include_directories(tengine/include/)

add_library(tengine SHARED IMPORTED)
set_target_properties(tengine PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/tengine/lib/libtengine-lite.so)

add_executable(face-recognizer-tengine-camera src/main.cpp src/UltraFace.cpp)
target_link_libraries(face-recognizer-tengine-camera tengine ${OpenCV_LIBS})
