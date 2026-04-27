cmake_minimum_required(VERSION 3.10)
project(app_height)

add_executable(app_height app_height.cpp)
target_link_libraries(app_height unitree_sdk2)

add_executable(textstate textstate.cpp)
target_link_libraries(textstate unitree_sdk2)