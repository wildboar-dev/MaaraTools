#--------------------------------------------------------
# Top-Level: NVLib
#
# @author: Wild Boar
#
# @Date Created: 2021-08-24
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.0.0)
project(NVLib VERSION 0.1.0)

enable_testing()

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # So that we can link against modules

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(NVLib)
