#include "Histogram.hpp"
#include "GaussianFilter.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

int main() {
    std::cout << "=== 高斯滤波示例 ===\n\n";
    
    // 创建直方图
    std::cout << "1. 创建直方图（范围[0, 20]，40个bin）...\n";
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    // 生成多峰值数据
    std::mt19937 gen(12345);
    std::normal_distribution<float> dist1(5.0f, 1.0f);
    std::normal_distribution<float> dist2(10.0f, 1.5f);
    std::normal_distribution<float> dist3(15.0f, 0.8f);
    
    for (int i = 0; i < 500; ++i) hist.addData(dist1(gen));
    for (int i = 0; i < 800; ++i) hist.addData(dist2(gen));
    for (int i = 0; i < 600; ++i) hist.addData(dist3(gen));
    
    std::cout << "   数据点数: " << hist.getTotalCount() << "\n";
    std::cout << "   bin数量: " << hist.getResolution() << "\n";
    std::cout << "   bin宽度: " << hist.getBinWidth() << "\n";
    
    // 显示原始bin计数（部分）
    std::cout << "\n2. 原始bin计数（前15个）:\n";
    std::cout << "   Bin索引 | 原始计数\n";
    std::cout << "   -------------------\n";
    for (size_t i = 0; i < std::min(hist.getResolution(), size_t(15)); ++i) {
        std::cout << "   " << std::setw(8) << i << " | " 
                  << std::setw(8) << hist.getBinCount(i) << "\n";
    }
    
    // 测试不同sigma值的高斯滤波
    std::cout << "\n3. 应用不同sigma值的高斯滤波:\n";
    
    std::vector<float> sigmaValues = {0.5f, 1.0f, 2.0f, 3.0f};
    for (float sigma : sigmaValues) {
        std::cout << "\n   Sigma = " << sigma << ":\n";
        histogram::GaussianFilter filter(sigma);
        auto filtered = filter.filterCounts(hist.getBinCounts());
        
        // 显示滤波后的值（部分）
        std::cout << "   Bin索引 | 滤波后值\n";
        std::cout << "   -------------------\n";
        for (size_t i = 0; i < std::min(filtered.size(), size_t(10)); ++i) {
            std::cout << "   " << std::setw(8) << i << " | " 
                      << std::setw(10) << std::fixed << std::setprecision(2) << filtered[i] << "\n";
        }
        
        // 计算滤波前后的总计数对比
        float originalSum = 0.0f;
        float filteredSum = 0.0f;
        for (size_t i = 0; i < hist.getResolution(); ++i) {
            originalSum += static_cast<float>(hist.getBinCount(i));
            filteredSum += filtered[i];
        }
        
        std::cout << "   原始总计数: " << originalSum << "\n";
        std::cout << "   滤波后总和: " << filteredSum << "\n";
        std::cout << "   总和变化: " << (filteredSum - originalSum) << "\n";
    }
    
    // 演示高斯核生成
    std::cout << "\n4. 高斯核生成演示:\n";
    histogram::GaussianFilter filterDemo(1.5f);
    
    // 注意：GaussianFilter类没有公开generateKernel方法，所以我们无法直接调用
    // 但我们可以通过观察滤波效果来理解
    
    // 测试边界情况
    std::cout << "\n5. 边界情况测试:\n";
    
    // 测试空直方图
    std::cout << "   a) 空直方图滤波:\n";
    histogram::Histogram emptyHist(0.0f, 10.0f, 10);
    histogram::GaussianFilter filter2(1.0f);
    auto emptyFiltered = filter2.filterCounts(emptyHist.getBinCounts());
    std::cout << "      滤波结果大小: " << emptyFiltered.size() << "\n";
    
    // 测试单个bin的直方图
    std::cout << "   b) 单个非零bin的直方图滤波:\n";
    histogram::Histogram singleHist(0.0f, 10.0f, 5);
    singleHist.addData(5.0f);
    auto singleFiltered = filter2.filterCounts(singleHist.getBinCounts());
    std::cout << "      原始计数: ";
    for (size_t i = 0; i < singleHist.getResolution(); ++i) {
        std::cout << singleHist.getBinCount(i) << " ";
    }
    std::cout << "\n      滤波后值: ";
    for (size_t i = 0; i < singleFiltered.size(); ++i) {
        std::cout << std::fixed << std::setprecision(2) << singleFiltered[i] << " ";
    }
    std::cout << "\n";
    
    // 演示sigma对平滑程度的影响
    std::cout << "\n6. Sigma对平滑程度的影响:\n";
    std::cout << "   Sigma越小，滤波核越窄，平滑效果越弱（保留更多细节）\n";
    std::cout << "   Sigma越大，滤波核越宽，平滑效果越强（丢失更多细节）\n";
    
    // 实际应用：峰值检测前的预处理
    std::cout << "\n7. 实际应用：峰值检测前的预处理\n";
    histogram::GaussianFilter smoothFilter(1.0f);
    auto smoothed = smoothFilter.filterCounts(hist.getBinCounts());
    
    // 将滤波后的浮点数转换回整数计数（近似）
    std::vector<size_t> smoothedCounts;
    for (float val : smoothed) {
        smoothedCounts.push_back(static_cast<size_t>(std::round(val)));
    }
    
    // 创建临时直方图来检测峰值
    histogram::Histogram smoothedHist(hist.getMin(), hist.getMax(), hist.getResolution());
    // 注意：这里不能直接设置bin计数，需要重新添加数据
    // 为了演示，我们只显示滤波后的值
    
    std::cout << "   原始数据峰值数量: " << hist.findPeaks(0.1f).size() << "\n";
    std::cout << "   （注意：这里无法直接检测滤波后数据的峰值，需要重新构建直方图）\n";
    
    std::cout << "\n=== 高斯滤波示例完成 ===\n";
    std::cout << "总结：高斯滤波可用于平滑直方图数据，减少噪声，便于后续分析。\n";
    std::cout << "关键参数sigma控制平滑程度：sigma越大，平滑效果越强。\n";
    
    return 0;
}
