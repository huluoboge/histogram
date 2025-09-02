#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <vector>
#include <stdexcept>

namespace histogram {

class Histogram {
public:
    /**
     * @brief 构造函数，初始化直方图
     * @param min 最小值
     * @param max 最大值
     * @param resolution 分辨率（bin数量）
     */
    Histogram(float min, float max, size_t resolution);
    
    /**
     * @brief 添加数据点到直方图
     * @param value 数据值
     */
    void addData(float value);
    
    /**
     * @brief 获取指定bin的计数值
     * @param binIndex bin索引
     * @return bin的计数值
     */
    size_t getBinCount(size_t binIndex) const;
    
    /**
     * @brief 获取指定bin的值范围
     * @param binIndex bin索引
     * @return bin的值范围（最小值，最大值）
     */
    std::pair<float, float> getBinRange(size_t binIndex) const;
    
    /**
     * @brief 获取bin数量
     * @return bin数量
     */
    size_t getResolution() const { return resolution_; }
    
    /**
     * @brief 获取最小值
     * @return 最小值
     */
    float getMin() const { return min_; }
    
    /**
     * @brief 获取最大值
     * @return 最大值
     */
    float getMax() const { return max_; }
    
    /**
     * @brief 获取bin宽度
     * @return bin宽度
     */
    float getBinWidth() const { return binWidth_; }
    
    /**
     * @brief 获取所有bin的计数值
     * @return bin计数向量
     */
    const std::vector<size_t>& getBinCounts() const { return bins_; }
    
    /**
     * @brief 获取总数据点数
     * @return 总数据点数
     */
    size_t getTotalCount() const { return totalCount_; }
    
    /**
     * @brief 清除所有数据
     */
    void clear();
    
    /**
     * @brief 获取数据点的bin索引
     * @param value 数据值
     * @return bin索引，如果超出范围返回-1
     */
    int getBinIndex(float value) const;

private:
    float min_;                 // 最小值
    float max_;                 // 最大值
    size_t resolution_;         // 分辨率（bin数量）
    float binWidth_;           // bin宽度
    std::vector<size_t> bins_; // bin计数
    size_t totalCount_;        // 总数据点数
};

} // namespace histogram

#endif // HISTOGRAM_HPP
