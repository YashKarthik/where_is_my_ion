cmake_minimum_required(VERSION 3.10)
project(grt_gtfs_realtime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
find_package(CURL REQUIRED)

add_subdirectory(src)
