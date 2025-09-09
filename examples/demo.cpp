#include "histogram/Histogram.hpp"
#include "histogram/CDF.hpp"
#include "histogram/GaussianFilter.hpp"
#include "histogram/SVGExporter.hpp"
#include <iostream>
#include <random>
#include <vector>

int main() {
    std::cout << "=== Histogram Library Demo ===\n\n";
    
    // 1. 创建直方图
    std::cout << "1. Creating histogram with range [0, 10] and 20 bins...\n";
    histogram::Histogram hist(0.0f, 10.0f, 20);
    
    // 2. 生成一些测试数据（正态分布）
    std::cout << "2. Generating normally distributed data (mean=5, stddev=2)...\n";
    std::mt19937 gen(42); // 固定种子以便重现结果
    std::normal_distribution<float> dist(5.0f, 2.0f);
    
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dist(gen));
    }
    
    std::cout << "   Added " << hist.getTotalCount() << " data points\n";
    
    // 3. 显示一些统计信息
    std::cout << "3. Histogram statistics:\n";
    std::cout << "   - Resolution: " << hist.getResolution() << " bins\n";
    std::cout << "   - Bin width: " << hist.getBinWidth() << "\n";
    std::cout << "   - Range: [" << hist.getMin() << ", " << hist.getMax() << "]\n";
    
    // 显示最大bin信息
    auto maxBin = hist.getMaxBin();
    auto binRange = hist.getBinRange(maxBin.second);
    std::cout << "   - Max bin count: " << maxBin.first << " (bin " << maxBin.second 
              << ", range [" << binRange.first << ", " << binRange.second << "])\n";
    
    // 4. 计算CDF
    std::cout << "4. Computing Cumulative Distribution Function...\n";
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    // 显示一些百分位
    std::cout << "   Percentiles:\n";
    std::cout << "   - 10th percentile: " << cdf.getPercentile(10.0f) << "\n";
    std::cout << "   - 50th percentile (median): " << cdf.getPercentile(50.0f) << "\n";
    std::cout << "   - 90th percentile: " << cdf.getPercentile(90.0f) << "\n";
    
    // 5. 应用高斯滤波
    std::cout << "5. Applying Gaussian filter (sigma=0.8)...\n";
    histogram::GaussianFilter filter(0.8f);
    auto filtered = filter.filterCounts(hist.getBinCounts());
    
    // 6. 导出SVG可视化
    std::cout << "6. Exporting visualizations to SVG files...\n";
    
    try {
        // 导出原始直方图
        histogram::SVGExporter::exportHistogram(hist, "demo_histogram.svg", 
                                              800, 600, "Normal Distribution Histogram");
        std::cout << "   - Original histogram: demo_histogram.svg\n";
        
        // 导出CDF
        histogram::SVGExporter::exportCDF(cdf, "demo_cdf.svg",
                                        800, 600, "Cumulative Distribution Function");
        std::cout << "   - CDF: demo_cdf.svg\n";
        
        // 导出滤波后的直方图
        histogram::SVGExporter::exportFilteredHistogram(hist, filtered, "demo_filtered.svg",
                                                      800, 600, "Gaussian Filtered Histogram");
        std::cout << "   - Filtered histogram: demo_filtered.svg\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error exporting SVG: " << e.what() << "\n";
        return 1;
    }
    
    // 7. 演示边界情况处理
    std::cout << "\n7. Demonstrating edge case handling:\n";
    
    // 测试超出范围的数据
    histogram::Histogram testHist(0.0f, 5.0f, 5);
    testHist.addData(-1.0f); // 应该被忽略
    testHist.addData(6.0f);  // 应该被忽略
    testHist.addData(2.5f);  // 有效数据
    
    std::cout << "   - Data points outside range are ignored\n";
    std::cout << "   - Valid data points counted: " << testHist.getTotalCount() << "\n";
    
    std::cout << "\n=== Demo completed successfully! ===\n";
    std::cout << "Check the generated SVG files for visualizations.\n";
    
    return 0;
}
