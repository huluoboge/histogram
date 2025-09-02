#include "histogram/CDF.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

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

void CDF::clear() {
    cdf_.clear();
    min_ = 0.0f;
    max_ = 0.0f;
    binWidth_ = 0.0f;
    resolution_ = 0;
}

} // namespace histogram
