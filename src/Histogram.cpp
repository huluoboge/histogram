#include "Histogram.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>

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

std::pair<size_t, size_t> Histogram::getMaxBin() const {
    if (bins_.empty()) {
        return {0, 0};
    }
    
    size_t maxCount = 0;
    size_t maxIndex = 0;
    
    for (size_t i = 0; i < bins_.size(); ++i) {
        if (bins_[i] > maxCount) {
            maxCount = bins_[i];
            maxIndex = i;
        }
    }
    
    return {maxCount, maxIndex};
}

size_t Histogram::getMaxBinCount() const {
    return getMaxBin().first;
}

size_t Histogram::getMaxBinIndex() const {
    return getMaxBin().second;
}

void Histogram::merge(const Histogram& other) {
    // 创建一个新的临时直方图，范围是两个直方图的并集
    float newMin = std::min(min_, other.min_);
    float newMax = std::max(max_, other.max_);
    
    // 如果范围没有变化，直接合并bins
    if (newMin == min_ && newMax == max_ && 
        std::abs((newMax - newMin) / resolution_ - other.binWidth_) < 1e-9) {
        // 两个直方图具有相同范围和bin宽度，可以直接合并
        for (size_t i = 0; i < bins_.size(); ++i) {
            bins_[i] += other.bins_[i];
        }
        totalCount_ += other.totalCount_;
        return;
    }
    
    // 需要重新分配bins
    size_t newResolution = resolution_;
    float newBinWidth = (newMax - newMin) / newResolution;
    
    std::vector<size_t> newBins(newResolution, 0);
    size_t newTotalCount = 0;
    
    // 将当前直方图的数据重新分配到新bins中
    for (size_t i = 0; i < resolution_; ++i) {
        size_t count = bins_[i];
        if (count > 0) {
            // 获取当前bin的中心点
            float binMin = min_ + i * binWidth_;
            float binMax = (i == resolution_ - 1) ? max_ : binMin + binWidth_;
            float binCenter = (binMin + binMax) / 2.0f;
            
            // 计算在新直方图中的索引
            int newIndex = static_cast<int>((binCenter - newMin) / newBinWidth);
            if (newIndex >= 0 && newIndex < static_cast<int>(newResolution)) {
                newBins[newIndex] += count;
                newTotalCount += count;
            } else {
                newTotalCount += count; // 仍然计入总数量，即使在范围外
            }
        }
    }
    
    // 将另一个直方图的数据也重新分配到新bins中
    for (size_t i = 0; i < other.resolution_; ++i) {
        size_t count = other.bins_[i];
        if (count > 0) {
            // 获取other直方图当前bin的中心点
            float binMin = other.min_ + i * other.binWidth_;
            float binMax = (i == other.resolution_ - 1) ? other.max_ : binMin + other.binWidth_;
            float binCenter = (binMin + binMax) / 2.0f;
            
            // 计算在新直方图中的索引
            int newIndex = static_cast<int>((binCenter - newMin) / newBinWidth);
            if (newIndex >= 0 && newIndex < static_cast<int>(newResolution)) {
                newBins[newIndex] += count;
                newTotalCount += count;
            } else {
                newTotalCount += count; // 仍然计入总数量，即使在范围外
            }
        }
    }
    
    // 更新当前直方图的参数
    min_ = newMin;
    max_ = newMax;
    binWidth_ = newBinWidth;
    bins_ = std::move(newBins);
    totalCount_ = newTotalCount;
}

std::vector<size_t> Histogram::findPeaks(float minProminence) const {
    std::vector<size_t> peaks;
    
    if (bins_.empty() || resolution_ < 3) {
        return peaks; // 数据不足，无法检测波峰
    }
    
    // 计算最小突出度阈值
    size_t maxCount = getMaxBinCount();
    size_t prominenceThreshold = static_cast<size_t>(maxCount * minProminence);
    
    // 计算平均计数，用于噪声过滤
    double averageCount = static_cast<double>(totalCount_) / resolution_;
    
    // 检测波峰：一个点比左右邻居都高
    for (size_t i = 1; i < resolution_ - 1; ++i) {
        if (bins_[i] > bins_[i-1] && bins_[i] > bins_[i+1]) {
            // 检查突出度是否满足阈值
            if (bins_[i] >= prominenceThreshold) {
                // 额外的检查：确保这不是噪声
                // 波峰应该显著高于周围的值（至少比两侧的平均值高10%）
                double neighborAverage = (bins_[i-1] + bins_[i+1]) / 2.0;
                if (bins_[i] > neighborAverage * 1.1) {
                    // 同时检查波峰计数应该高于整体平均值
                    if (bins_[i] > averageCount * 1.5) {
                        peaks.push_back(i);
                    }
                }
            }
        }
    }
    
    return peaks;
}

std::vector<std::tuple<size_t, size_t, std::pair<float, float>>> Histogram::getPeaksInfo(float minProminence) const {
    std::vector<std::tuple<size_t, size_t, std::pair<float, float>>> peaksInfo;
    
    auto peaks = findPeaks(minProminence);
    for (size_t peakIndex : peaks) {
        size_t count = bins_[peakIndex];
        auto range = getBinRange(peakIndex);
        peaksInfo.emplace_back(peakIndex, count, range);
    }
    
    return peaksInfo;
}

} // namespace histogram