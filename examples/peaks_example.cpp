#include "Histogram.hpp"
#include <iostream>
#include <random>
#include <tuple>

int main() {
    // 创建一个直方图
    histogram::Histogram hist(0.0f, 100.0f, 50);
    
    // 添加一些具有明显峰值的测试数据
    // 第一个峰值在值约为20的位置
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dis1(20.0f, 3.0f); // 峰值1在20附近
    std::normal_distribution<float> dis2(50.0f, 4.0f); // 峰值2在50附近
    std::normal_distribution<float> dis3(80.0f, 2.0f); // 峰值3在80附近
    
    // 添加数据形成多个峰值
    for (int i = 0; i < 2000; ++i) {
        hist.addData(dis1(gen)); // 在20附近添加2000个点
    }
    
    for (int i = 0; i < 1500; ++i) {
        hist.addData(dis2(gen)); // 在50附近添加1500个点
    }
    
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dis3(gen)); // 在80附近添加1000个点
    }
    
    // 查找峰值
    auto peaks = hist.getPeaksInfo(0.1f); // 使用10%的最小突出度阈值
    
    std::cout << "找到 " << peaks.size() << " 个峰值" << std::endl;
    
    // 获取第一个波峰
    if (!peaks.empty()) {
        auto firstPeak = peaks[0];
        size_t peakIndex = std::get<0>(firstPeak);
        size_t peakCount = std::get<1>(firstPeak);
        auto peakRange = std::get<2>(firstPeak);
        
        std::cout << "\n第一个波峰信息:" << std::endl;
        std::cout << "  索引: " << peakIndex << std::endl;
        std::cout << "  计数: " << peakCount << std::endl;
        std::cout << "  范围: [" << peakRange.first << ", " << peakRange.second << ")" << std::endl;
        std::cout << "  中心值: " << (peakRange.first + peakRange.second) / 2 << std::endl;
    } else {
        std::cout << "未找到任何波峰" << std::endl;
    }
    
    // 显示所有波峰信息
    std::cout << "\n所有波峰信息:" << std::endl;
    for (size_t i = 0; i < peaks.size(); ++i) {
        auto peak = peaks[i];
        size_t peakIndex = std::get<0>(peak);
        size_t peakCount = std::get<1>(peak);
        auto peakRange = std::get<2>(peak);
        
        std::cout << "波峰 " << (i+1) << ":" << std::endl;
        std::cout << "  索引: " << peakIndex << std::endl;
        std::cout << "  计数: " << peakCount << std::endl;
        std::cout << "  范围: [" << peakRange.first << ", " << peakRange.second << ")" << std::endl;
        std::cout << "  中心值: " << (peakRange.first + peakRange.second) / 2 << std::endl;
    }
    
    // 显示直方图数据以便更好地理解
    std::cout << "\n直方图数据:" << std::endl;
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        auto range = hist.getBinRange(i);
        size_t count = hist.getBinCount(i);
        float center = (range.first + range.second) / 2;
        
        // 只显示有数据的bins或者包含波峰的bins
        if (count > 0 || i < 10 || (i > 15 && i < 25) || (i > 45 && i < 55) || (i > 75 && i < 85)) {
            std::cout << "Bin " << i << " (中心值: " << center << "): " << count << " 个数据点" << std::endl;
        } else if (i == 10 || i == 25 || i == 55 || i == 85) {
            std::cout << "..." << std::endl; // 省略无数据的区间
        }
    }
    
    return 0;
}
