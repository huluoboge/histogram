#ifndef GAUSSIAN_FILTER_HPP
#define GAUSSIAN_FILTER_HPP

#include <vector>
#include <cmath>

namespace histogram {

class GaussianFilter {
public:
    /**
     * @brief 构造函数
     * @param sigma 高斯核的标准差
     */
    GaussianFilter(float sigma = 1.0f);
    
    /**
     * @brief 设置高斯核的标准差
     * @param sigma 标准差
     */
    void setSigma(float sigma);
    
    /**
     * @brief 对一维数据进行高斯滤波
     * @param input 输入数据
     * @return 滤波后的数据
     */
    std::vector<float> filter(const std::vector<float>& input) const;
    
    /**
     * @brief 对直方图计数进行高斯滤波
     * @param counts 直方图计数
     * @return 滤波后的计数
     */
    std::vector<float> filterCounts(const std::vector<size_t>& counts) const;

private:
    /**
     * @brief 生成高斯核
     * @param kernelSize 核大小
     * @return 高斯核
     */
    std::vector<float> generateKernel(int kernelSize) const;
    
    float sigma_; // 高斯核标准差
};

} // namespace histogram

#endif // GAUSSIAN_FILTER_HPP
