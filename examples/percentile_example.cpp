#include "Histogram.hpp"
#include "CDF.hpp"
#include <iostream>
#include <random>

int main() {
    // 创建一个直方图
    histogram::Histogram hist(0.0f, 100.0f, 50);
    
    // 生成一些测试数据（正态分布）
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dis(50.0f, 15.0f); // 均值50，标准差15
    
    // 添加10000个数据点到直方图
    for (int i = 0; i < 10000; ++i) {
        hist.addData(dis(gen));
    }
    
    // 计算CDF
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    // 获取5%分位数（第5百分位）
    float percentile5 = cdf.getPercentile(5.0f);
    std::cout << "5% percentile value: " << percentile5 << std::endl;
    
    // 获取其他常用百分位数进行对比
    float percentile25 = cdf.getPercentile(25.0f);
    float percentile50 = cdf.getPercentile(50.0f); // 中位数
    float percentile75 = cdf.getPercentile(75.0f);
    float percentile95 = cdf.getPercentile(95.0f);
    
    std::cout << "25% percentile value: " << percentile25 << std::endl;
    std::cout << "50% percentile value (median): " << percentile50 << std::endl;
    std::cout << "75% percentile value: " << percentile75 << std::endl;
    std::cout << "95% percentile value: " << percentile95 << std::endl;
    
    // 验证CDF是归一化的
    const auto& cdfValues = cdf.getCDFValues();
    std::cout << "\nCDF normalization check:" << std::endl;
    std::cout << "First CDF value: " << cdfValues.front() << std::endl;
    std::cout << "Last CDF value: " << cdfValues.back() << std::endl;
    
    // 显示一些CDF值
    std::cout << "\nSample CDF values:" << std::endl;
    for (size_t i = 0; i < cdfValues.size(); i += 10) {
        auto range = hist.getBinRange(i);
        std::cout << "Bin " << i << " (value around " << (range.first + range.second)/2 
                  << "): CDF = " << cdfValues[i] << std::endl;
    }
    
    return 0;
}
