cmake_minimum_required(VERSION 2.8)
project(startProject)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_executable(startProject ${CMAKE_SOURCE_DIR}/src/vidDisplay.cpp ${CMAKE_SOURCE_DIR}/src/filter.cpp)
target_link_libraries(startProject ${OpenCV_LIBS})