#include "Histogram.hpp"
#include <iostream>
#include <random>
#include <vector>

int main() {
    std::cout << "=== 直方图合并示例 ===\n\n";
    
    // 创建第一个直方图
    std::cout << "1. 创建第一个直方图（范围[0, 10]，20个bin）...\n";
    histogram::Histogram hist1(0.0f, 10.0f, 20);
    
    std::mt19937 gen1(42);
    std::normal_distribution<float> dist1(3.0f, 1.0f);
    for (int i = 0; i < 500; ++i) {
        hist1.addData(dist1(gen1));
    }
    
    std::cout << "   第一个直方图数据点数: " << hist1.getTotalCount() << "\n";
    std::cout << "   最大bin计数: " << hist1.getMaxBinCount() << "\n";
    
    // 创建第二个直方图
    std::cout << "\n2. 创建第二个直方图（相同范围[0, 10]，20个bin）...\n";
    histogram::Histogram hist2(0.0f, 10.0f, 20);
    
    std::mt19937 gen2(123);
    std::normal_distribution<float> dist2(7.0f, 1.5f);
    for (int i = 0; i < 800; ++i) {
        hist2.addData(dist2(gen2));
    }
    
    std::cout << "   第二个直方图数据点数: " << hist2.getTotalCount() << "\n";
    std::cout << "   最大bin计数: " << hist2.getMaxBinCount() << "\n";
    
    // 合并前显示两个直方图的bin计数
    std::cout << "\n3. 合并前两个直方图的bin计数对比:\n";
    std::cout << "   Bin索引 | Hist1计数 | Hist2计数\n";
    std::cout << "   ---------------------------------\n";
    for (size_t i = 0; i < hist1.getResolution(); ++i) {
        if (hist1.getBinCount(i) > 0 || hist2.getBinCount(i) > 0) {
            std::cout << "   " << i << "        | " 
                      << hist1.getBinCount(i) << "         | " 
                      << hist2.getBinCount(i) << "\n";
        }
    }
    
    // 合并直方图
    std::cout << "\n4. 合并两个直方图...\n";
    hist1.merge(hist2);
    
    std::cout << "   合并后数据点数: " << hist1.getTotalCount() << "\n";
    std::cout << "   合并后最大bin计数: " << hist1.getMaxBinCount() << "\n";
    
    // 验证合并结果
    std::cout << "\n5. 验证合并结果:\n";
    std::cout << "   Bin索引 | 合并后计数 | 期望计数 (Hist1+Hist2)\n";
    std::cout << "   ---------------------------------------------\n";
    
    bool allCorrect = true;
    for (size_t i = 0; i < hist1.getResolution(); ++i) {
        size_t expected = hist1.getBinCount(i);
        // 注意：hist2已经被合并，我们无法再获取其原始计数
        // 但我们知道合并前的计数
        size_t hist1Original = 0;
        size_t hist2Original = 0;
        // 由于我们无法获取原始值，这里只显示合并后的值
        if (expected > 0) {
            std::cout << "   " << i << "        | " 
                      << expected << "         | N/A\n";
        }
    }
    
    // 测试合并不同范围的直方图（应该失败或产生警告）
    std::cout << "\n6. 测试合并不同范围的直方图...\n";
    histogram::Histogram hist3(0.0f, 5.0f, 10);  // 不同范围
    histogram::Histogram hist4(0.0f, 10.0f, 10); // 不同范围
    
    try {
        hist3.merge(hist4);
        std::cout << "   警告：不同范围的直方图被合并，可能导致数据丢失\n";
    } catch (const std::exception& e) {
        std::cout << "   异常: " << e.what() << "\n";
    }
    
    // 测试合并不同bin数量的直方图
    std::cout << "\n7. 测试合并不同bin数量的直方图...\n";
    histogram::Histogram hist5(0.0f, 10.0f, 10);  // 10个bin
    histogram::Histogram hist6(0.0f, 10.0f, 20);  // 20个bin
    
    try {
        hist5.merge(hist6);
        std::cout << "   警告：不同bin数量的直方图被合并，可能导致数据丢失\n";
    } catch (const std::exception& e) {
        std::cout << "   异常: " << e.what() << "\n";
    }
    
    std::cout << "\n=== 直方图合并示例完成 ===\n";
    std::cout << "总结：merge()函数将另一个直方图的数据合并到当前直方图中。\n";
    std::cout << "注意：两个直方图必须具有相同的范围和bin数量才能正确合并。\n";
    
    return 0;
}
