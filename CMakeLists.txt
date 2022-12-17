cmake_minimum_required(VERSION 3.23)
project(ya_order)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

add_executable(ya_order main.cpp include/Server.h include/Client.h include/Item.h src/Server.cpp src/Item.cpp src/Client.cpp include/Utils.h)
target_link_libraries (${PROJECT_NAME} pthread)