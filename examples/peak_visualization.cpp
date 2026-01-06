#include "Histogram.hpp"
#include "SVGExporter.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <tuple>

int main() {
    std::cout << "=== 波峰检测可视化示例 ===\n\n";
    
    // 创建直方图，范围[0, 20]，40个bin
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    // 生成多峰值数据（三个正态分布的混合）
    std::mt19937 gen(12345); // 固定种子以便重现结果
    
    // 第一个峰值：均值=4，标准差=0.8
    std::normal_distribution<float> dist1(4.0f, 0.8f);
    for (int i = 0; i < 400; ++i) {
        hist.addData(dist1(gen));
    }
    
    // 第二个峰值：均值=10，标准差=1.2
    std::normal_distribution<float> dist2(10.0f, 1.2f);
    for (int i = 0; i < 600; ++i) {
        hist.addData(dist2(gen));
    }
    
    // 第三个峰值：均值=16，标准差=0.9
    std::normal_distribution<float> dist3(16.0f, 0.9f);
    for (int i = 0; i < 500; ++i) {
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
        for (const auto& peak : peaksInfo) {
            size_t index = std::get<0>(peak);
            size_t count = std::get<1>(peak);
            auto range = std::get<2>(peak);
            float center = (range.first + range.second) / 2.0f;
            
            std::cout << "  - 索引: " << index 
                      << ", 计数值: " << count
                      << ", 范围: [" << range.first << ", " << range.second << "]"
                      << ", 中心值: " << center << "\n";
        }
    }
    
    // 导出带波峰标记的SVG
    try {
        histogram::SVGExporter::exportHistogramWithPeaks(hist, peaksInfo, 
                                                       "peak_visualization.svg",
                                                       800, 600, 
                                                       "Histogram with Detected Peaks");
        std::cout << "\nSVG图像已导出: peak_visualization.svg\n";
        
        // 同时导出普通直方图用于对比
        histogram::SVGExporter::exportHistogram(hist, "normal_histogram.svg",
                                              800, 600, "Normal Histogram");
        std::cout << "普通直方图已导出: normal_histogram.svg\n";
        
    } catch (const std::exception& e) {
        std::cerr << "导出SVG时出错: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\n=== 可视化完成 ===\n";
    std::cout << "请查看生成的SVG文件来观察波峰检测结果。\n";
    
    return 0;
}
