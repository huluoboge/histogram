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
     * @brief 获取指定百分位对应的bin索引
     * @param percentile 百分位 [0, 100]
     * @return bin索引，如果无效则返回-1
     */
    int getBinIndexForPercentile(float percentile) const;
    
    /**
     * @brief 获取指定百分位对应的bin范围
     * @param percentile 百分位 [0, 100]
     * @return bin范围对 (min, max)，如果无效则返回(0, 0)
     */
    std::pair<float, float> getBinRangeForPercentile(float percentile) const;
    
    /**
     * @brief 获取CDF值向量
     * @return CDF值向量
     */
    const std::vector<float>& getCDFValues() const { return cdf_; }
    
    /**
     * @brief 清除CDF数据
     */
    void clear();
    
    /**
     * @brief 打印直方图和CDF的详细信息到控制台
     * @param hist 直方图对象
     * @param cdf CDF对象
     * @param showAll 是否显示所有bin（包括计数为0的）
     */
    static void printHistogramAndCDF(const Histogram& hist, 
                                    const CDF& cdf,
                                    bool showAll = false);
    
    /**
     * @brief 导出直方图和CDF的详细信息到文本文件
     * @param hist 直方图对象
     * @param cdf CDF对象
     * @param filename 输出文件名
     * @param showAll 是否显示所有bin（包括计数为0的）
     */
    static void exportHistogramAndCDFToFile(const Histogram& hist,
                                           const CDF& cdf,
                                           const std::string& filename,
                                           bool showAll = false);

private:
    std::vector<float> cdf_; // 累计分布值
    float min_;              // 最小值
    float max_;              // 最大值
    float binWidth_;         // bin宽度
    size_t resolution_;      // 分辨率
};

} // namespace histogram

#endif // CDF_HPP
