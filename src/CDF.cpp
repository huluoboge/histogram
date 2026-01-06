#include "CDF.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace histogram {

void CDF::computeFromHistogram(const Histogram& hist) {
    const auto& binCounts = hist.getBinCounts();
    size_t totalCount = hist.getTotalCount();
    
    if (totalCount == 0) {
        throw std::runtime_error("Histogram has no data");
    }
    
    resolution_ = hist.getResolution();
    min_ = hist.getMin();
    max_ = hist.getMax();
    binWidth_ = hist.getBinWidth();
    
    cdf_.resize(resolution_);
    
    // 计算累计分布
    float cumulative = 0.0f;
    for (size_t i = 0; i < resolution_; ++i) {
        cumulative += static_cast<float>(binCounts[i]) / totalCount;
        cdf_[i] = cumulative;
    }
    
    // 确保最后一个值为1.0（由于浮点精度问题）
    if (resolution_ > 0) {
        cdf_[resolution_ - 1] = 1.0f;
    }
}

float CDF::getCumulativeProbability(float value) const {
    if (value < min_) return 0.0f;
    if (value >= max_) return 1.0f;
    
    int binIndex = static_cast<int>((value - min_) / binWidth_);
    binIndex = std::min(binIndex, static_cast<int>(resolution_ - 1));
    binIndex = std::max(binIndex, 0);
    
    return cdf_[binIndex];
}

float CDF::getPercentile(float percentile) const {
    if (percentile < 0.0f || percentile > 100.0f) {
        throw std::invalid_argument("Percentile must be between 0 and 100");
    }
    
    if (cdf_.empty()) {
        throw std::runtime_error("CDF not computed");
    }
    
    float target = percentile / 100.0f;
    
    // 找到第一个累计概率大于等于目标值的bin
    for (size_t i = 0; i < resolution_; ++i) {
        if (cdf_[i] >= target) {
            // 线性插值
            float binMin = min_ + i * binWidth_;
            float binMax = (i == resolution_ - 1) ? max_ : binMin + binWidth_;
            
            // 计算在bin内的位置
            float prevCDF = (i > 0) ? cdf_[i - 1] : 0.0f;
            float fraction = (target - prevCDF) / (cdf_[i] - prevCDF);
            return binMin + fraction * binWidth_;
        }
    }
    
    return max_;
}

int CDF::getBinIndexForPercentile(float percentile) const {
    if (percentile < 0.0f || percentile > 100.0f) {
        return -1;
    }
    
    if (cdf_.empty()) {
        return -1;
    }
    
    float target = percentile / 100.0f;
    
    // 找到第一个累计概率大于等于目标值的bin
    for (size_t i = 0; i < resolution_; ++i) {
        if (cdf_[i] >= target) {
            return static_cast<int>(i);
        }
    }
    
    // 如果所有CDF值都小于目标值（理论上不应该发生，因为最后一个CDF值应为1.0）
    return static_cast<int>(resolution_ - 1);
}

std::pair<float, float> CDF::getBinRangeForPercentile(float percentile) const {
    int binIndex = getBinIndexForPercentile(percentile);
    if (binIndex < 0 || binIndex >= static_cast<int>(resolution_)) {
        return {0.0f, 0.0f};
    }
    
    float binMin = min_ + binIndex * binWidth_;
    float binMax = (binIndex == static_cast<int>(resolution_) - 1) ? max_ : binMin + binWidth_;
    
    return {binMin, binMax};
}

void CDF::clear() {
    cdf_.clear();
    min_ = 0.0f;
    max_ = 0.0f;
    binWidth_ = 0.0f;
    resolution_ = 0;
}

void CDF::printHistogramAndCDF(const Histogram& hist, 
                              const CDF& cdf,
                              bool showAll) {
    const auto& cdfValues = cdf.getCDFValues();
    
    std::cout << "   bin索引 | 计数 | bin范围           | CDF值    | 累计概率%\n";
    std::cout << "   --------|------|-------------------|----------|----------\n";
    
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        auto range = hist.getBinRange(i);
        size_t count = hist.getBinCount(i);
        float cdfValue = cdfValues[i];
        
        if (showAll || count > 0 || i < 5 || i > hist.getResolution() - 5) {
            std::cout << "   " << std::setw(7) << i << " | "
                      << std::setw(4) << count << " | "
                      << "[" << std::setw(6) << std::fixed << std::setprecision(1) << range.first
                      << ", " << std::setw(6) << std::fixed << std::setprecision(1) << range.second << ") | "
                      << std::setw(8) << std::fixed << std::setprecision(4) << cdfValue << " | "
                      << std::setw(8) << std::fixed << std::setprecision(1) << (cdfValue * 100) << "%\n";
        } else if (i == 5) {
            std::cout << "   ... (中间bin省略) ...\n";
        }
    }
}

void CDF::exportHistogramAndCDFToFile(const Histogram& hist,
                                     const CDF& cdf,
                                     const std::string& filename,
                                     bool showAll) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    
    const auto& cdfValues = cdf.getCDFValues();
    
    file << "bin索引,计数,bin最小值,bin最大值,CDF值,累计概率%\n";
    
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        auto range = hist.getBinRange(i);
        size_t count = hist.getBinCount(i);
        float cdfValue = cdfValues[i];
        
        if (showAll || count > 0) {
            file << i << ","
                 << count << ","
                 << std::fixed << std::setprecision(2) << range.first << ","
                 << std::fixed << std::setprecision(2) << range.second << ","
                 << std::fixed << std::setprecision(6) << cdfValue << ","
                 << std::fixed << std::setprecision(2) << (cdfValue * 100) << "%\n";
        }
    }
    
    file.close();
    std::cout << "   数据已导出到文件: " << filename << std::endl;
}

} // namespace histogram
