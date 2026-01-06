#include "Histogram.hpp"
#include "CDF.hpp"
#include "GaussianFilter.hpp"
#include "SVGExporter.hpp"
#include <iostream>
#include <limits>
#include <cmath>

int main() {
    std::cout << "=== 边界情况处理示例 ===\n\n";
    
    // 测试1: 极端范围直方图
    std::cout << "1. 极端范围直方图测试:\n";
    
    // 非常小的范围
    std::cout << "   a) 非常小的范围 [0.0, 0.001]:\n";
    try {
        histogram::Histogram tinyHist(0.0f, 0.001f, 10);
        std::cout << "      创建成功，bin宽度: " << tinyHist.getBinWidth() << "\n";
        
        // 添加接近边界的数据
        tinyHist.addData(0.0f);
        tinyHist.addData(0.0005f);
        tinyHist.addData(0.001f);
        std::cout << "      数据点数: " << tinyHist.getTotalCount() << "\n";
    } catch (const std::exception& e) {
        std::cout << "      错误: " << e.what() << "\n";
    }
    
    // 非常大的范围
    std::cout << "   b) 非常大的范围 [0.0, 1e6]:\n";
    try {
        histogram::Histogram hugeHist(0.0f, 1e6f, 100);
        std::cout << "      创建成功，bin宽度: " << hugeHist.getBinWidth() << "\n";
        
        // 添加数据
        hugeHist.addData(500000.0f);
        hugeHist.addData(750000.0f);
        std::cout << "      数据点数: " << hugeHist.getTotalCount() << "\n";
    } catch (const std::exception& e) {
        std::cout << "      错误: " << e.what() << "\n";
    }
    
    // 测试2: 单个bin的直方图
    std::cout << "\n2. 单个bin的直方图测试:\n";
    try {
        histogram::Histogram singleBinHist(0.0f, 10.0f, 1);
        std::cout << "      创建成功，bin数量: " << singleBinHist.getResolution() << "\n";
        std::cout << "      bin宽度: " << singleBinHist.getBinWidth() << "\n";
        
        // 添加数据
        for (int i = 0; i < 10; ++i) {
            singleBinHist.addData(5.0f);
        }
        std::cout << "      数据点数: " << singleBinHist.getTotalCount() << "\n";
        std::cout << "      bin计数: " << singleBinHist.getBinCount(0) << "\n";
        
        // 测试峰值检测
        auto peaks = singleBinHist.findPeaks();
        std::cout << "      检测到的峰值数量: " << peaks.size() << "\n";
    } catch (const std::exception& e) {
        std::cout << "      错误: " << e.what() << "\n";
    }
    
    // 测试3: 空直方图操作
    std::cout << "\n3. 空直方图操作测试:\n";
    histogram::Histogram emptyHist(0.0f, 10.0f, 10);
    
    std::cout << "   a) 获取最大bin: ";
    try {
        auto maxBin = emptyHist.getMaxBin();
        std::cout << "计数=" << maxBin.first << ", 索引=" << maxBin.second << "\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    std::cout << "   b) 峰值检测: ";
    auto peaks = emptyHist.findPeaks();
    std::cout << "检测到 " << peaks.size() << " 个峰值\n";
    
    std::cout << "   c) 计算CDF: ";
    histogram::CDF emptyCDF;
    emptyCDF.computeFromHistogram(emptyHist);
    std::cout << "CDF值数量: " << emptyCDF.getCDFValues().size() << "\n";
    
    // 测试4: 无效数据点处理
    std::cout << "\n4. 无效数据点处理测试:\n";
    histogram::Histogram validHist(0.0f, 10.0f, 10);
    
    // 添加NaN
    std::cout << "   a) 添加NaN: ";
    validHist.addData(std::numeric_limits<float>::quiet_NaN());
    std::cout << "数据点数: " << validHist.getTotalCount() << " (应忽略NaN)\n";
    
    // 添加无穷大
    std::cout << "   b) 添加正无穷大: ";
    validHist.addData(std::numeric_limits<float>::infinity());
    std::cout << "数据点数: " << validHist.getTotalCount() << " (应忽略无穷大)\n";
    
    // 添加负无穷大
    std::cout << "   c) 添加负无穷大: ";
    validHist.addData(-std::numeric_limits<float>::infinity());
    std::cout << "数据点数: " << validHist.getTotalCount() << " (应忽略无穷大)\n";
    
    // 添加超出范围的数据
    std::cout << "   d) 添加超出范围的数据: ";
    size_t beforeCount = validHist.getTotalCount();
    validHist.addData(-5.0f);  // 小于最小值
    validHist.addData(15.0f);  // 大于最大值
    size_t afterCount = validHist.getTotalCount();
    std::cout << "添加前后数据点数: " << beforeCount << " -> " << afterCount 
              << " (应保持不变)\n";
    
    // 测试5: 边界值数据
    std::cout << "\n5. 边界值数据测试:\n";
    histogram::Histogram edgeHist(0.0f, 10.0f, 10);
    
    // 添加正好等于最小值的数据
    std::cout << "   a) 添加最小值(0.0): ";
    edgeHist.addData(0.0f);
    std::cout << "bin索引: " << edgeHist.getBinIndex(0.0f) << "\n";
    
    // 添加正好等于最大值的数据
    std::cout << "   b) 添加最大值(10.0): ";
    edgeHist.addData(10.0f);
    std::cout << "bin索引: " << edgeHist.getBinIndex(10.0f) << "\n";
    
    // 添加非常接近最大值但小于最大值的数据
    std::cout << "   c) 添加接近最大值的数据(9.9999): ";
    edgeHist.addData(9.9999f);
    std::cout << "bin索引: " << edgeHist.getBinIndex(9.9999f) << "\n";
    
    // 测试6: 高斯滤波边界情况
    std::cout << "\n6. 高斯滤波边界情况测试:\n";
    
    // 测试sigma为0
    std::cout << "   a) Sigma = 0: ";
    try {
        histogram::GaussianFilter zeroSigmaFilter(0.0f);
        std::cout << "创建成功\n";
        
        std::vector<size_t> counts = {1, 2, 3, 4, 5};
        auto filtered = zeroSigmaFilter.filterCounts(counts);
        std::cout << "      滤波结果大小: " << filtered.size() << "\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    // 测试非常大的sigma
    std::cout << "   b) Sigma = 100: ";
    try {
        histogram::GaussianFilter largeSigmaFilter(100.0f);
        std::cout << "创建成功\n";
        
        std::vector<size_t> counts = {1, 0, 0, 0, 1};
        auto filtered = largeSigmaFilter.filterCounts(counts);
        std::cout << "      滤波结果: ";
        for (float val : filtered) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    // 测试7: CDF边界情况
    std::cout << "\n7. CDF边界情况测试:\n";
    
    // 测试所有数据在第一个bin
    std::cout << "   a) 所有数据在第一个bin:\n";
    histogram::Histogram firstBinHist(0.0f, 10.0f, 5);
    for (int i = 0; i < 100; ++i) firstBinHist.addData(0.1f);
    
    histogram::CDF firstBinCDF;
    firstBinCDF.computeFromHistogram(firstBinHist);
    
    try {
        float p0 = firstBinCDF.getPercentile(0.0f);
        float p100 = firstBinCDF.getPercentile(100.0f);
        std::cout << "      0%分位数: " << p0 << "\n";
        std::cout << "      100%分位数: " << p100 << "\n";
    } catch (const std::exception& e) {
        std::cout << "      错误: " << e.what() << "\n";
    }
    
    // 测试8: 内存和性能边界
    std::cout << "\n8. 内存和性能边界测试:\n";
    
    // 测试大量bin
    std::cout << "   a) 大量bin (10000个): ";
    try {
        histogram::Histogram manyBinsHist(0.0f, 100.0f, 10000);
        std::cout << "创建成功\n";
        std::cout << "      bin宽度: " << manyBinsHist.getBinWidth() << "\n";
        
        // 添加一些数据
        manyBinsHist.addData(50.0f);
        std::cout << "      数据点数: " << manyBinsHist.getTotalCount() << "\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    // 测试大量数据点
    std::cout << "   b) 大量数据点 (100000个): ";
    try {
        histogram::Histogram manyDataHist(0.0f, 1.0f, 100);
        
        // 添加大量数据
        for (int i = 0; i < 100000; ++i) {
            manyDataHist.addData(0.5f);
        }
        std::cout << "添加成功\n";
        std::cout << "      数据点数: " << manyDataHist.getTotalCount() << "\n";
        std::cout << "      最大bin计数: " << manyDataHist.getMaxBinCount() << "\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    // 测试9: SVG导出边界情况
    std::cout << "\n9. SVG导出边界情况测试:\n";
    
    // 测试空直方图导出
    std::cout << "   a) 空直方图SVG导出: ";
    try {
        histogram::SVGExporter::exportHistogram(emptyHist, "empty_histogram.svg", 
                                              800, 600, "Empty Histogram");
        std::cout << "成功创建 empty_histogram.svg\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    // 测试非常小的图像尺寸
    std::cout << "   b) 小尺寸图像导出 (10x10): ";
    try {
        histogram::Histogram smallHist(0.0f, 10.0f, 5);
        smallHist.addData(5.0f);
        
        histogram::SVGExporter::exportHistogram(smallHist, "small_histogram.svg", 
                                              10, 10, "Small Histogram");
        std::cout << "成功创建 small_histogram.svg\n";
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << "\n";
    }
    
    std::cout << "\n=== 边界情况处理示例完成 ===\n";
    std::cout << "总结：库应能正确处理各种边界情况，包括极端参数、无效输入和边缘条件。\n";
    std::cout << "注意：实际行为可能因实现而异，上述测试展示了预期的健壮性。\n";
    
    return 0;
}
