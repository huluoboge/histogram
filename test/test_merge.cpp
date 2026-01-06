#include "Histogram.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Testing histogram merge functionality..." << std::endl;
    
    // 测试1: 合并相同范围的直方图
    std::cout << "\n--- Test 1: Merging histograms with same range ---" << std::endl;
    histogram::Histogram hist1(0.0f, 10.0f, 5);
    histogram::Histogram hist2(0.0f, 10.0f, 5);
    
    // 向第一个直方图添加数据
    hist1.addData(1.0f);
    hist1.addData(2.0f);
    hist1.addData(2.5f);
    hist1.addData(3.0f);
    
    // 向第二个直方图添加数据
    hist2.addData(1.5f);
    hist2.addData(2.5f);
    hist2.addData(7.0f);
    hist2.addData(8.0f);
    hist2.addData(9.0f);
    
    std::cout << "Before merge:" << std::endl;
    std::cout << "Hist1 range: [" << hist1.getMin() << ", " << hist1.getMax() << "], total count: " << hist1.getTotalCount() << std::endl;
    std::cout << "Hist2 range: [" << hist2.getMin() << ", " << hist2.getMax() << "], total count: " << hist2.getTotalCount() << std::endl;
    
    for (size_t i = 0; i < hist1.getResolution(); ++i) {
        auto range = hist1.getBinRange(i);
        std::cout << "Bin " << i << " (" << range.first << "-" << range.second << "): " << hist1.getBinCount(i) << std::endl;
    }
    
    // 合并直方图
    hist1.merge(hist2);
    
    std::cout << "\nAfter merge:" << std::endl;
    std::cout << "Hist1 range: [" << hist1.getMin() << ", " << hist1.getMax() << "], total count: " << hist1.getTotalCount() << std::endl;
    
    for (size_t i = 0; i < hist1.getResolution(); ++i) {
        auto range = hist1.getBinRange(i);
        std::cout << "Bin " << i << " (" << range.first << "-" << range.second << "): " << hist1.getBinCount(i) << std::endl;
    }
    
    // 测试2: 合并不同范围的直方图
    std::cout << "\n--- Test 2: Merging histograms with different ranges ---" << std::endl;
    histogram::Histogram hist3(0.0f, 10.0f, 5);
    histogram::Histogram hist4(5.0f, 15.0f, 5);
    
    // 向第三个直方图添加数据
    hist3.addData(1.0f);
    hist3.addData(2.0f);
    hist3.addData(5.0f);
    hist3.addData(8.0f);
    
    // 向第四个直方图添加数据
    hist4.addData(6.0f);
    hist4.addData(7.0f);
    hist4.addData(12.0f);
    hist4.addData(14.0f);
    
    std::cout << "Before merge:" << std::endl;
    std::cout << "Hist3 range: [" << hist3.getMin() << ", " << hist3.getMax() << "], total count: " << hist3.getTotalCount() << std::endl;
    std::cout << "Hist4 range: [" << hist4.getMin() << ", " << hist4.getMax() << "], total count: " << hist4.getTotalCount() << std::endl;
    
    // 合并直方图
    hist3.merge(hist4);
    
    std::cout << "\nAfter merge:" << std::endl;
    std::cout << "Hist3 range: [" << hist3.getMin() << ", " << hist3.getMax() << "], total count: " << hist3.getTotalCount() << std::endl;
    
    for (size_t i = 0; i < hist3.getResolution(); ++i) {
        auto range = hist3.getBinRange(i);
        std::cout << "Bin " << i << " (" << range.first << "-" << range.second << "): " << hist3.getBinCount(i) << std::endl;
    }
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}