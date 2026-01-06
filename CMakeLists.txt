cmake_minimum_required(VERSION 3.12)
project(histogram VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(CMAKE_BUILD_TYPE)
    message(STATUS "Build type specified as ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Build type not specified, using Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(IS_DEBUG TRUE)
else()
    set(IS_DEBUG FALSE)
endif()

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

# 添加包含目录
include_directories(src)


# 创建库
add_library(histogram
    src/Histogram.cpp
    src/CDF.cpp
    src/GaussianFilter.cpp
    src/SVGExporter.cpp
)

# 启用测试
enable_testing()

# 查找GoogleTest
find_package(GTest REQUIRED)

add_subdirectory(test)
add_subdirectory(examples)
# 安装配置
install(TARGETS histogram DESTINATION lib)
install(DIRECTORY include/histogram DESTINATION include)
