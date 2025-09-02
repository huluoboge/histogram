#include "histogram/Histogram.hpp"
#include <cmath>
#include <algorithm>

namespace histogram {

Histogram::Histogram(float min, float max, size_t resolution)
    : min_(min), max_(max), resolution_(resolution), totalCount_(0) {
    
    if (min >= max) {
        throw std::invalid_argument("min must be less than max");
    }
    if (resolution == 0) {
        throw std::invalid_argument("resolution must be greater than 0");
    }
    
    binWidth_ = (max - min) / resolution;
    bins_.resize(resolution, 0);
}

void Histogram::addData(float value) {
    int binIndex = getBinIndex(value);
    if (binIndex >= 0 && binIndex < static_cast<int>(resolution_)) {
        bins_[binIndex]++;
        totalCount_++;
    }
    // 忽略超出范围的值
}

size_t Histogram::getBinCount(size_t binIndex) const {
    if (binIndex >= resolution_) {
        throw std::out_of_range("binIndex out of range");
    }
    return bins_[binIndex];
}

std::pair<float, float> Histogram::getBinRange(size_t binIndex) const {
    if (binIndex >= resolution_) {
        throw std::out_of_range("binIndex out of range");
    }
    
    float binMin = min_ + binIndex * binWidth_;
    float binMax = (binIndex == resolution_ - 1) ? max_ : binMin + binWidth_;
    
    return {binMin, binMax};
}

void Histogram::clear() {
    std::fill(bins_.begin(), bins_.end(), 0);
    totalCount_ = 0;
}

int Histogram::getBinIndex(float value) const {
    if (value < min_ || value > max_) {
        return -1; // 超出范围
    }
    
    if (value == max_) {
        return resolution_ - 1; // 最大值属于最后一个bin
    }
    
    int index = static_cast<int>((value - min_) / binWidth_);
    return std::min(index, static_cast<int>(resolution_ - 1));
}

} // namespace histogram
