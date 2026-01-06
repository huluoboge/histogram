#include "Histogram.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

void testSinglePeak() {
    std::cout << "=== 测试1: 单峰值直方图 ===\n";
    histogram::Histogram hist(0.0f, 10.0f, 20);
    
    std::mt19937 gen(42);
    std::normal_distribution<float> dist(5.0f, 1.0f);
    
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dist(gen));
    }
    
    auto peaks = hist.findPeaks(0.05f);
    std::cout << "检测到 " << peaks.size() << " 个波峰\n";
    
    if (!peaks.empty()) {
        auto range = hist.getBinRange(peaks[0]);
        std::cout << "主波峰在范围 [" << range.first << ", " << range.second << "]\n";
    }
    std::cout << "\n";
}

void testMultiplePeaks() {
    std::cout << "=== 测试2: 多峰值直方图 ===\n";
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    std::mt19937 gen(123);
    
    // 三个不同的正态分布
    std::normal_distribution<float> dist1(4.0f, 0.8f);
    std::normal_distribution<float> dist2(10.0f, 1.2f);
    std::normal_distribution<float> dist3(16.0f, 0.9f);
    
    for (int i = 0; i < 400; ++i) hist.addData(dist1(gen));
    for (int i = 0; i < 600; ++i) hist.addData(dist2(gen));
    for (int i = 0; i < 500; ++i) hist.addData(dist3(gen));
    
    std::cout << "不同突出度阈值的检测结果:\n";
    
    // 测试不同的突出度阈值
    std::vector<float> thresholds = {0.05f, 0.1f, 0.2f, 0.3f};
    for (float threshold : thresholds) {
        auto peaks = hist.findPeaks(threshold);
        std::cout << "阈值 " << threshold * 100 << "%: " << peaks.size() << " 个波峰\n";
    }
    std::cout << "\n";
}

void testNoPeaks() {
    std::cout << "=== 测试3: 无峰值数据（均匀分布） ===\n";
    histogram::Histogram hist(0.0f, 10.0f, 20);
    
    std::mt19937 gen(456);
    std::uniform_real_distribution<float> dist(0.0f, 10.0f);
    
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dist(gen));
    }
    
    auto peaks = hist.findPeaks(0.1f);
    std::cout << "检测到 " << peaks.size() << " 个波峰 (应为0)\n\n";
}

void testEdgeCases() {
    std::cout << "=== 测试4: 边界情况 ===\n";
    
    // 测试空直方图
    histogram::Histogram emptyHist(0.0f, 10.0f, 10);
    auto emptyPeaks = emptyHist.findPeaks();
    std::cout << "空直方图检测到 " << emptyPeaks.size() << " 个波峰\n";
    
    // 测试只有很少bin的直方图
    histogram::Histogram smallHist(0.0f, 10.0f, 2);
    smallHist.addData(2.5f);
    smallHist.addData(7.5f);
    auto smallPeaks = smallHist.findPeaks();
    std::cout << "小直方图检测到 " << smallPeaks.size() << " 个波峰\n\n";
}

int main() {
    std::cout << "=== 高级波峰检测测试 ===\n\n";
    
    testSinglePeak();
    testMultiplePeaks();
    testNoPeaks();
    testEdgeCases();
    
    std::cout << "=== 所有测试完成 ===\n";
    return 0;
}
