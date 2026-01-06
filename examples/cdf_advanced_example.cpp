#include "Histogram.hpp"
#include "CDF.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <cmath>

int main() {
    std::cout << "=== CDF高级功能示例 ===\n\n";
    
    // 创建直方图
    std::cout << "1. 创建直方图（范围[0, 100]，50个bin）...\n";
    histogram::Histogram hist(0.0f, 100.0f, 50);
    
    // 生成混合分布数据
    std::mt19937 gen(12345);
    
    // 第一个正态分布：均值30，标准差10
    std::normal_distribution<float> dist1(30.0f, 10.0f);
    for (int i = 0; i < 2000; ++i) hist.addData(dist1(gen));
    
    // 第二个正态分布：均值70，标准差15
    std::normal_distribution<float> dist2(70.0f, 15.0f);
    for (int i = 0; i < 3000; ++i) hist.addData(dist2(gen));
    
    std::cout << "   数据点数: " << hist.getTotalCount() << "\n";
    std::cout << "   bin数量: " << hist.getResolution() << "\n";
    
    // 计算CDF
    std::cout << "\n2. 计算累计分布函数(CDF)...\n";
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    // 测试getCumulativeProbability函数
    std::cout << "\n3. 测试getCumulativeProbability函数:\n";
    std::vector<float> testValues = {0.0f, 25.0f, 50.0f, 75.0f, 100.0f, 150.0f};
    
    std::cout << "   值       | 累计概率\n";
    std::cout << "   ---------|----------\n";
    for (float value : testValues) {
        try {
            float prob = cdf.getCumulativeProbability(value);
            std::cout << "   " << std::setw(8) << value << " | " 
                      << std::setw(10) << std::fixed << std::setprecision(4) << prob << "\n";
        } catch (const std::exception& e) {
            std::cout << "   " << std::setw(8) << value << " | 错误: " << e.what() << "\n";
        }
    }
    
    // 测试getPercentile函数
    std::cout << "\n4. 测试getPercentile函数:\n";
    std::vector<float> percentiles = {0.0f, 10.0f, 25.0f, 50.0f, 75.0f, 90.0f, 95.0f, 99.0f, 100.0f};
    
    std::cout << "   百分位   | 对应值\n";
    std::cout << "   ---------|----------\n";
    for (float p : percentiles) {
        try {
            float value = cdf.getPercentile(p);
            std::cout << "   " << std::setw(8) << p << "% | " 
                      << std::setw(10) << std::fixed << std::setprecision(2) << value << "\n";
        } catch (const std::exception& e) {
            std::cout << "   " << std::setw(8) << p << "% | 错误: " << e.what() << "\n";
        }
    }
    
    // 验证CDF属性
    std::cout << "\n5. 验证CDF数学属性:\n";
    
    const auto& cdfValues = cdf.getCDFValues();
    if (!cdfValues.empty()) {
        // 属性1: CDF值应在[0, 1]范围内
        bool validRange = true;
        for (float val : cdfValues) {
            if (val < 0.0f || val > 1.0f) {
                validRange = false;
                break;
            }
        }
        std::cout << "   a) CDF值范围[0, 1]: " << (validRange ? "通过" : "失败") << "\n";
        
        // 属性2: CDF应是非递减的
        bool nonDecreasing = true;
        for (size_t i = 1; i < cdfValues.size(); ++i) {
            if (cdfValues[i] < cdfValues[i-1]) {
                nonDecreasing = false;
                break;
            }
        }
        std::cout << "   b) CDF非递减性: " << (nonDecreasing ? "通过" : "失败") << "\n";
        
        // 属性3: 第一个CDF值应为0或接近0
        std::cout << "   c) 第一个CDF值: " << cdfValues.front() << "\n";
        
        // 属性4: 最后一个CDF值应为1或接近1
        std::cout << "   d) 最后一个CDF值: " << cdfValues.back() << "\n";
        
        // 属性5: CDF值数量应与bin数量相同
        std::cout << "   e) CDF值数量: " << cdfValues.size() 
                  << " (bin数量: " << hist.getResolution() << ")\n";
    }
    
    // 演示CDF的实际应用
    std::cout << "\n6. CDF实际应用示例:\n";
    
    // 应用1: 确定数据的中间80%范围
    float p10 = cdf.getPercentile(10.0f);
    float p90 = cdf.getPercentile(90.0f);
    std::cout << "   a) 数据的中间80%范围: [" << p10 << ", " << p90 << "]\n";
    
    // 应用2: 检测异常值（低于1%或高于99%）
    float p1 = cdf.getPercentile(1.0f);
    float p99 = cdf.getPercentile(99.0f);
    std::cout << "   b) 异常值边界: 低于" << p1 << " 或高于" << p99 << "\n";
    
    // 应用3: 计算数据落在某个区间的概率
    float intervalStart = 40.0f;
    float intervalEnd = 60.0f;
    float probStart = cdf.getCumulativeProbability(intervalStart);
    float probEnd = cdf.getCumulativeProbability(intervalEnd);
    float intervalProb = probEnd - probStart;
    std::cout << "   c) 数据落在[" << intervalStart << ", " << intervalEnd 
              << "]区间的概率: " << intervalProb * 100 << "%\n";
    
    // 测试边界情况
    std::cout << "\n7. 边界情况测试:\n";
    
    // 测试空直方图的CDF
    std::cout << "   a) 空直方图的CDF:\n";
    histogram::Histogram emptyHist(0.0f, 10.0f, 10);
    histogram::CDF emptyCDF;
    emptyCDF.computeFromHistogram(emptyHist);
    std::cout << "      CDF值数量: " << emptyCDF.getCDFValues().size() << "\n";
    
    // 测试所有数据在一个bin的直方图
    std::cout << "   b) 所有数据在一个bin的直方图:\n";
    histogram::Histogram singleBinHist(0.0f, 10.0f, 5);
    for (int i = 0; i < 100; ++i) singleBinHist.addData(5.0f);
    histogram::CDF singleBinCDF;
    singleBinCDF.computeFromHistogram(singleBinHist);
    
    try {
        float median = singleBinCDF.getPercentile(50.0f);
        std::cout << "      中位数: " << median << "\n";
    } catch (const std::exception& e) {
        std::cout << "      错误: " << e.what() << "\n";
    }
    
    // 测试均匀分布
    std::cout << "   c) 均匀分布的CDF:\n";
    histogram::Histogram uniformHist(0.0f, 10.0f, 10);
    std::uniform_real_distribution<float> uniformDist(0.0f, 10.0f);
    for (int i = 0; i < 1000; ++i) uniformHist.addData(uniformDist(gen));
    
    histogram::CDF uniformCDF;
    uniformCDF.computeFromHistogram(uniformHist);
    
    // 均匀分布的CDF应近似线性
    float uniformP25 = uniformCDF.getPercentile(25.0f);
    float uniformP50 = uniformCDF.getPercentile(50.0f);
    float uniformP75 = uniformCDF.getPercentile(75.0f);
    
    std::cout << "      25%分位数: " << uniformP25 << " (期望: ~2.5)\n";
    std::cout << "      50%分位数: " << uniformP50 << " (期望: ~5.0)\n";
    std::cout << "      75%分位数: " << uniformP75 << " (期望: ~7.5)\n";
    
    std::cout << "\n=== CDF高级功能示例完成 ===\n";
    std::cout << "总结：CDF提供了数据的完整分布信息，可用于分位数计算、概率估计和异常值检测。\n";
    
    return 0;
}
