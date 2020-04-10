cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(graphisoft_racer)

add_executable(graphisoft_racer src/car.cpp src/driving.cpp src/map.cpp src/vector.cpp src/main.cpp)

# Ez miert nem mukodik? Mindegy, majd Adam megszereli
set_target_properties(graphisoft_racer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
