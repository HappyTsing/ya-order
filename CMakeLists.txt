cmake_minimum_required(VERSION 3.23)
project(ya_order)

set(CMAKE_CXX_STANDARD 14)

#add_executable(ya_order main.cpp include/Server.h include/StoreClient.h include/StudentClient.h include/Item.h src/Server.cpp src/Item.cpp src/StoreClient.cpp src/StudentClient.cpp)
add_executable(ya_order main.cpp include/Server.h src/Server.cpp)
