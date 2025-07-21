cmake_minimum_required(VERSION 3.15)

project(OpenMap)


add_executable(
        OpenMap
        src/main.cpp
)

target_include_directories(
        OpenMap
        PUBLIC .
)

