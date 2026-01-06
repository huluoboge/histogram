# Examples for histogram library

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加所有示例程序
add_executable(demo demo.cpp)
target_link_libraries(demo histogram)

add_executable(peaks_example peaks_example.cpp)
target_link_libraries(peaks_example histogram)

add_executable(sorted_peaks_example sorted_peaks_example.cpp)
target_link_libraries(sorted_peaks_example histogram)

add_executable(peak_detection peak_detection.cpp)
target_link_libraries(peak_detection histogram)

add_executable(peak_detection_advanced peak_detection_advanced.cpp)
target_link_libraries(peak_detection_advanced histogram)

add_executable(peak_visualization peak_visualization.cpp)
target_link_libraries(peak_visualization histogram)

add_executable(percentile_example percentile_example.cpp)
target_link_libraries(percentile_example histogram)

# 新添加的示例程序
add_executable(merge_example merge_example.cpp)
target_link_libraries(merge_example histogram)

add_executable(gaussian_filter_example gaussian_filter_example.cpp)
target_link_libraries(gaussian_filter_example histogram)

add_executable(cdf_advanced_example cdf_advanced_example.cpp)
target_link_libraries(cdf_advanced_example histogram)

add_executable(edge_cases_example edge_cases_example.cpp)
target_link_libraries(edge_cases_example histogram)

add_executable(percentile_bin_example percentile_bin_example.cpp)
target_link_libraries(percentile_bin_example histogram)

# 安装示例程序（可选）
if(INSTALL_EXAMPLES)
    install(TARGETS 
        demo
        peaks_example
        sorted_peaks_example
        peak_detection
        peak_detection_advanced
        peak_visualization
        percentile_example
        merge_example
        gaussian_filter_example
        cdf_advanced_example
        edge_cases_example
        percentile_bin_example
        DESTINATION bin)
endif()
