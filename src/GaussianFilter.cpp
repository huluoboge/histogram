#include "GaussianFilter.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace histogram {

GaussianFilter::GaussianFilter(float sigma) : sigma_(sigma) {
    if (sigma <= 0) {
        throw std::invalid_argument("Sigma must be greater than 0");
    }
}

void GaussianFilter::setSigma(float sigma) {
    if (sigma <= 0) {
        throw std::invalid_argument("Sigma must be greater than 0");
    }
    sigma_ = sigma;
}

std::vector<float> GaussianFilter::filter(const std::vector<float>& input) const {
    if (input.empty()) {
        return {};
    }
    
    // 计算核大小（3倍sigma，确保覆盖99.7%的数据）
    int kernelSize = static_cast<int>(std::ceil(3 * sigma_)) * 2 + 1;
    auto kernel = generateKernel(kernelSize);
    
    std::vector<float> output(input.size(), 0.0f);
    int radius = kernelSize / 2;
    
    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        float sum = 0.0f;
        float weightSum = 0.0f;
        
        for (int j = -radius; j <= radius; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < static_cast<int>(input.size())) {
                float weight = kernel[j + radius];
                sum += input[idx] * weight;
                weightSum += weight;
            }
        }
        
        output[i] = (weightSum > 0) ? sum / weightSum : 0.0f;
    }
    
    return output;
}

std::vector<float> GaussianFilter::filterCounts(const std::vector<size_t>& counts) const {
    // 将size_t转换为float进行滤波
    std::vector<float> floatCounts(counts.begin(), counts.end());
    return filter(floatCounts);
}

std::vector<float> GaussianFilter::generateKernel(int kernelSize) const {
    if (kernelSize % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }
    
    std::vector<float> kernel(kernelSize);
    int radius = kernelSize / 2;
    float sum = 0.0f;
    
    // 生成高斯核
    for (int i = -radius; i <= radius; ++i) {
        float x = static_cast<float>(i);
        float value = std::exp(-(x * x) / (2 * sigma_ * sigma_));
        kernel[i + radius] = value;
        sum += value;
    }
    
    // 归一化
    for (auto& value : kernel) {
        value /= sum;
    }
    
    return kernel;
}

} // namespace histogram
