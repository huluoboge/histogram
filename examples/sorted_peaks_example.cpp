#include "Histogram.hpp"
#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>

int main() {
    // 创建一个直方图
    histogram::Histogram hist(0.0f, 100.0f, 50);
    
    // 添加一些具有明显峰值的测试数据
    // 第一个峰值在值约为80的位置（最高的峰值）
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dis1(20.0f, 3.0f); // 峰值1在20附近
    std::normal_distribution<float> dis2(50.0f, 4.0f); // 峰值2在50附近
    std::normal_distribution<float> dis3(80.0f, 2.0f); // 峰值3在80附近（最高的峰值）
    
    // 添加数据形成多个峰值，其中80附近的峰值最高
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dis1(gen)); // 在20附近添加1000个点
    }
    
    for (int i = 0; i < 1500; ++i) {
        hist.addData(dis2(gen)); // 在50附近添加1500个点
    }
    
    for (int i = 0; i < 2000; ++i) {
        hist.addData(dis3(gen)); // 在80附近添加2000个点（最高的峰值）
    }
    
    // 查找峰值（按显著性排序）
    auto peaks = hist.getPeaksInfo(0.1f); // 使用10%的最小突出度阈值
    
    std::cout << "按显著性排序的波峰:" << std::endl;
    std::cout << "找到 " << peaks.size() << " 个峰值" << std::endl;
    
    // 显示所有波峰信息（按显著性排序）
    for (size_t i = 0; i < peaks.size(); ++i) {
        auto peak = peaks[i];
        size_t peakIndex = std::get<0>(peak);
        size_t peakCount = std::get<1>(peak);
        auto peakRange = std::get<2>(peak);
        float centerValue = (peakRange.first + peakRange.second) / 2;
        
        std::cout << "波峰 " << (i+1) << ":" << std::endl;
        std::cout << "  索引: " << peakIndex << std::endl;
        std::cout << "  计数: " << peakCount << std::endl;
        std::cout << "  范围: [" << peakRange.first << ", " << peakRange.second << ")" << std::endl;
        std::cout << "  中心值: " << centerValue << std::endl;
    }
    
    // 按波峰的中心值排序（从小到大）
    std::sort(peaks.begin(), peaks.end(), 
        [](const auto& a, const auto& b) {
            float centerA = (std::get<2>(a).first + std::get<2>(a).second) / 2;
            float centerB = (std::get<2>(b).first + std::get<2>(b).second) / 2;
            return centerA < centerB;
        });
    
    std::cout << "\n按波峰中心值排序（从小到大）:" << std::endl;
    
    // 显示所有波峰信息（按中心值排序）
    for (size_t i = 0; i < peaks.size(); ++i) {
        auto peak = peaks[i];
        size_t peakIndex = std::get<0>(peak);
        size_t peakCount = std::get<1>(peak);
        auto peakRange = std::get<2>(peak);
        float centerValue = (peakRange.first + peakRange.second) / 2;
        
        std::cout << "波峰 " << (i+1) << ":" << std::endl;
        std::cout << "  索引: " << peakIndex << std::endl;
        std::cout << "  计数: " << peakCount << std::endl;
        std::cout << "  范围: [" << peakRange.first << ", " << peakRange.second << ")" << std::endl;
        std::cout << "  中心值: " << centerValue << std::endl;
    }
    
    // 获取第一个（最小）波峰
    if (!peaks.empty()) {
        auto firstSmallestPeak = peaks[0];
        size_t peakIndex = std::get<0>(firstSmallestPeak);
        size_t peakCount = std::get<1>(firstSmallestPeak);
        auto peakRange = std::get<2>(firstSmallestPeak);
        float centerValue = (peakRange.first + peakRange.second) / 2;
        
        std::cout << "\n最小的波峰:" << std::endl;
        std::cout << "  索引: " << peakIndex << std::endl;
        std::cout << "  计数: " << peakCount << std::endl;
        std::cout << "  范围: [" << peakRange.first << ", " << peakRange.second << ")" << std::endl;
        std::cout << "  中心值: " << centerValue << std::endl;
    }
    
    return 0;
}
