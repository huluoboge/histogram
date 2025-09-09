cmake_minimum_required(VERSION 3.12)
project(histogram VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加包含目录
include_directories(include)

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

# 添加测试可执行文件
add_executable(test_histogram test/test_histogram.cpp)
target_link_libraries(test_histogram histogram GTest::GTest GTest::Main)

# 添加测试
add_test(NAME histogram_tests COMMAND test_histogram)

# 添加示例
add_executable(demo examples/demo.cpp)
target_link_libraries(demo histogram)

add_executable(peak_detection examples/peak_detection.cpp)
target_link_libraries(peak_detection histogram)

add_executable(peak_detection_advanced examples/peak_detection_advanced.cpp)
target_link_libraries(peak_detection_advanced histogram)

add_executable(peak_visualization examples/peak_visualization.cpp)
target_link_libraries(peak_visualization histogram)

# 安装配置
install(TARGETS histogram DESTINATION lib)
install(DIRECTORY include/histogram DESTINATION include)
