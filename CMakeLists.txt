cmake_minimum_required(VERSION 3.5.0)
project(FluidSim VERSION 0.1.0 LANGUAGES C CXX)

add_executable(FluidSim main.cpp config.cpp point.cpp face.cpp cell.cpp volocity.cpp)

