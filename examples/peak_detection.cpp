#include "histogram/Histogram.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

int main() {
    std::cout << "=== 直方图波峰检测演示 ===\n\n";
    
    // 创建直方图，范围[0, 20]，40个bin
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    // 生成多峰值数据（三个正态分布的混合）
    std::mt19937 gen(12345); // 固定种子以便重现结果
    
    // 第一个峰值：均值=5，标准差=1
    std::normal_distribution<float> dist1(5.0f, 1.0f);
    for (int i = 0; i < 500; ++i) {
        hist.addData(dist1(gen));
    }
    
    // 第二个峰值：均值=10，标准差=1.5
    std::normal_distribution<float> dist2(10.0f, 1.5f);
    for (int i = 0; i < 800; ++i) {
        hist.addData(dist2(gen));
    }
    
    // 第三个峰值：均值=15，标准差=0.8
    std::normal_distribution<float> dist3(15.0f, 0.8f);
    for (int i = 0; i < 600; ++i) {
        hist.addData(dist3(gen));
    }
    
    std::cout << "添加了 " << hist.getTotalCount() << " 个数据点\n";
    std::cout << "bin数量: " << hist.getResolution() << "\n";
    std::cout << "bin宽度: " << hist.getBinWidth() << "\n\n";
    
    // 检测波峰
    std::cout << "检测波峰 (最小突出度阈值: 10%):\n";
    auto peaksInfo = hist.getPeaksInfo(0.1f);
    
    if (peaksInfo.empty()) {
        std::cout << "未检测到波峰\n";
    } else {
        std::cout << "检测到 " << peaksInfo.size() << " 个波峰:\n";
        std::cout << std::setw(8) << "索引" 
                  << std::setw(10) << "计数值" 
                  << std::setw(20) << "值范围" 
                  << std::setw(15) << "中心值" << "\n";
        std::cout << std::string(60, '-') << "\n";
        
        for (const auto& peak : peaksInfo) {
            size_t index = std::get<0>(peak);
            size_t count = std::get<1>(peak);
            auto range = std::get<2>(peak);
            float center = (range.first + range.second) / 2.0f;
            
            std::cout << std::setw(8) << index
                      << std::setw(10) << count
                      << std::setw(20) << "[" + std::to_string(range.first).substr(0, 4) + 
                                         ", " + std::to_string(range.second).substr(0, 4) + "]"
                      << std::setw(15) << std::fixed << std::setprecision(2) << center << "\n";
        }
    }
    
    std::cout << "\n=== 波峰检测完成 ===\n";
    
    // 显示所有bin的计数（用于调试）
    std::cout << "\n所有bin的计数（前20个）:\n";
    for (size_t i = 0; i < std::min(hist.getResolution(), size_t(20)); ++i) {
        std::cout << "bin " << i << ": " << hist.getBinCount(i) << "\n";
    }
    
    return 0;
}
