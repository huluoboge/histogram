#include "Histogram.hpp"
#include "CDF.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <cmath>

int main() {
    std::cout << "=== 百分位对应bin索引示例 ===\n\n";
    
    // 创建直方图
    std::cout << "1. 创建直方图（范围[0, 100]，20个bin）...\n";
    histogram::Histogram hist(0.0f, 100.0f, 20);
    
    // 生成正态分布数据
    std::mt19937 gen(12345);
    std::normal_distribution<float> dist(50.0f, 15.0f);
    
    for (int i = 0; i < 5000; ++i) {
        hist.addData(dist(gen));
    }
    
    std::cout << "   数据点数: " << hist.getTotalCount() << "\n";
    std::cout << "   bin数量: " << hist.getResolution() << "\n";
    std::cout << "   bin宽度: " << hist.getBinWidth() << "\n";
    
    // 计算CDF
    std::cout << "\n2. 计算CDF...\n";
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    // 测试不同百分位对应的bin
    std::cout << "\n3. 不同百分位对应的bin索引和范围:\n";
    std::vector<float> percentiles = {0.0f, 10.0f, 25.0f, 50.0f, 75.0f, 90.0f, 95.0f, 99.0f, 100.0f};
    
    std::cout << "   百分位   | bin索引 | bin范围           | 百分位值      | bin中心值\n";
    std::cout << "   ---------|---------|-------------------|--------------|----------\n";
    
    for (float p : percentiles) {
        // 获取bin索引（使用CDF类的新方法）
        int binIndex = cdf.getBinIndexForPercentile(p);
        
        // 获取bin范围（使用CDF类的新方法）
        auto binRange = cdf.getBinRangeForPercentile(p);
        
        // 获取百分位对应的值
        float percentileValue = cdf.getPercentile(p);
        
        // 计算bin中心值
        float binCenter = (binRange.first + binRange.second) / 2.0f;
        
        std::cout << "   " << std::setw(7) << p << "% | "
                  << std::setw(7) << binIndex << " | "
                  << "[" << std::setw(6) << std::fixed << std::setprecision(1) << binRange.first
                  << ", " << std::setw(6) << std::fixed << std::setprecision(1) << binRange.second << ") | "
                  << std::setw(12) << std::fixed << std::setprecision(2) << percentileValue << " | "
                  << std::setw(10) << std::fixed << std::setprecision(2) << binCenter << "\n";
    }
    
    // 验证：比较百分位值和bin中心值
    std::cout << "\n4. 验证：比较百分位值和bin中心值:\n";
    std::cout << "   注意：百分位值是通过线性插值计算的，可能不在bin中心。\n";
    
    // 显示bin的详细信息（使用CDF类的静态方法）
    std::cout << "\n5. 所有bin的详细信息（包含CDF值）:\n";
    histogram::CDF::printHistogramAndCDF(hist, cdf, false);
    
    // 导出数据到文件
    std::cout << "\n5b. 导出数据到文件...\n";
    histogram::CDF::exportHistogramAndCDFToFile(hist, cdf, "histogram_cdf_data.csv", false);
    
    // 实际应用示例：找到包含大多数数据的bin范围
    std::cout << "\n6. 实际应用：找到包含80%数据的bin范围:\n";
    
    // 找到第10百分位和第90百分位对应的bin
    int p10Bin = cdf.getBinIndexForPercentile(10.0f);
    int p90Bin = cdf.getBinIndexForPercentile(90.0f);
    
    if (p10Bin >= 0 && p90Bin >= 0) {
        auto range10 = hist.getBinRange(p10Bin);
        auto range90 = hist.getBinRange(p90Bin);
        
        std::cout << "   第10百分位在bin " << p10Bin << "，范围: [" 
                  << range10.first << ", " << range10.second << ")\n";
        std::cout << "   第90百分位在bin " << p90Bin << "，范围: [" 
                  << range90.first << ", " << range90.second << ")\n";
        
        // 计算包含的bin数量
        int binCount = std::abs(p90Bin - p10Bin) + 1;
        std::cout << "   包含80%数据的bin数量: " << binCount << " (总共" 
                  << hist.getResolution() << "个bin)\n";
        
        // 计算这些bin中的数据比例
        size_t totalInRange = 0;
        int startBin = std::min(p10Bin, p90Bin);
        int endBin = std::max(p10Bin, p90Bin);
        
        for (int i = startBin; i <= endBin; ++i) {
            totalInRange += hist.getBinCount(i);
        }
        
        float proportion = static_cast<float>(totalInRange) / hist.getTotalCount();
        std::cout << "   这些bin中的数据比例: " << proportion * 100 << "%\n";
    }
    
    // 边界情况测试
    std::cout << "\n7. 边界情况测试:\n";
    
    // 测试0%和100%
    std::cout << "   a) 0%对应的bin: " << cdf.getBinIndexForPercentile(0.0f) << "\n";
    std::cout << "   b) 100%对应的bin: " << cdf.getBinIndexForPercentile(100.0f) << "\n";
    
    // 测试无效百分位
    std::cout << "   c) -10%对应的bin: " << cdf.getBinIndexForPercentile(-10.0f) << " (应为-1)\n";
    std::cout << "   d) 110%对应的bin: " << cdf.getBinIndexForPercentile(110.0f) << " (应为-1)\n";
    
    // 测试空CDF
    histogram::CDF emptyCDF;
    std::cout << "   e) 空CDF的50%对应的bin: " << emptyCDF.getBinIndexForPercentile(50.0f) << " (应为-1)\n";
    
    std::cout << "\n=== 百分位对应bin索引示例完成 ===\n";
    std::cout << "总结：通过CDF可以找到任意百分位对应的bin索引，这对于理解数据分布很有用。\n";
    std::cout << "注意：百分位值可能落在bin内的任何位置，不一定在bin中心。\n";
    std::cout << "注意：现在这些功能已经集成到CDF类中，可以直接使用cdf.getBinIndexForPercentile()和cdf.getBinRangeForPercentile()方法。\n";
    
    return 0;
}
