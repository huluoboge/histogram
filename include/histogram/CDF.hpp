#ifndef CDF_HPP
#define CDF_HPP

#include "Histogram.hpp"
#include <vector>
#include <stdexcept>

namespace histogram {

class CDF {
public:
    /**
     * @brief 从直方图计算累计分布函数
     * @param hist 直方图对象
     */
    void computeFromHistogram(const Histogram& hist);
    
    /**
     * @brief 获取指定值的累计概率
     * @param value 数据值
     * @return 累计概率 [0, 1]
     */
    float getCumulativeProbability(float value) const;
    
    /**
     * @brief 获取指定百分位的值
     * @param percentile 百分位 [0, 100]
     * @return 对应的数据值
     */
    float getPercentile(float percentile) const;
    
    /**
     * @brief 获取CDF值向量
     * @return CDF值向量
     */
    const std::vector<float>& getCDFValues() const { return cdf_; }
    
    /**
     * @brief 清除CDF数据
     */
    void clear();

private:
    std::vector<float> cdf_; // 累计分布值
    float min_;              // 最小值
    float max_;              // 最大值
    float binWidth_;         // bin宽度
    size_t resolution_;      // 分辨率
};

} // namespace histogram

#endif // CDF_HPP
