cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME server)

set(CMAKE_BUILD_TYPE "Debug")

# 头文件指定所在位置
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

# 指定生成运行目标
add_executable(${PROJECT_NAME} server.cpp ${sources} ${headers})