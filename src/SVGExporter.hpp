#ifndef SVG_EXPORTER_HPP
#define SVG_EXPORTER_HPP

#include "Histogram.hpp"
#include "CDF.hpp"
#include <string>
#include <vector>

namespace histogram {

class SVGExporter {
public:
    /**
     * @brief 导出直方图到SVG
     * @param hist 直方图对象
     * @param filename 输出文件名
     * @param width SVG宽度
     * @param height SVG高度
     * @param title 图表标题
     */
    static void exportHistogram(const Histogram& hist, 
                               const std::string& filename,
                               int width = 800, 
                               int height = 600,
                               const std::string& title = "Histogram");
    
    /**
     * @brief 导出CDF到SVG
     * @param cdf CDF对象
     * @param filename 输出文件名
     * @param width SVG宽度
     * @param height SVG高度
     * @param title 图表标题
     */
    static void exportCDF(const CDF& cdf,
                         const std::string& filename,
                         int width = 800,
                         int height = 600,
                         const std::string& title = "Cumulative Distribution Function");
    
    /**
     * @brief 导出滤波后的直方图到SVG
     * @param hist 原始直方图
     * @param filtered 滤波后的数据
     * @param filename 输出文件名
     * @param width SVG宽度
     * @param height SVG高度
     * @param title 图表标题
     */
    static void exportFilteredHistogram(const Histogram& hist,
                                       const std::vector<float>& filtered,
                                       const std::string& filename,
                                       int width = 800,
                                       int height = 600,
                                       const std::string& title = "Filtered Histogram");
    
    /**
     * @brief 导出带波峰标记的直方图到SVG
     * @param hist 直方图对象
     * @param peaksInfo 波峰信息向量
     * @param filename 输出文件名
     * @param width SVG宽度
     * @param height SVG高度
     * @param title 图表标题
     */
    static void exportHistogramWithPeaks(const Histogram& hist,
                                        const std::vector<std::tuple<size_t, size_t, std::pair<float, float>>>& peaksInfo,
                                        const std::string& filename,
                                        int width = 800,
                                        int height = 600,
                                        const std::string& title = "Histogram with Peaks");

private:
    /**
     * @brief 创建SVG文件头
     * @param width 宽度
     * @param height 高度
     * @param title 标题
     * @return SVG头内容
     */
    static std::string createSVGHeader(int width, int height, const std::string& title);
    
    /**
     * @brief 创建SVG文件尾
     * @return SVG尾内容
     */
    static std::string createSVGFooter();
    
    /**
     * @brief 创建坐标轴
     * @param width 图表宽度
     * @param height 图表高度
     * @param margin 边距
     * @param xMin X轴最小值
     * @param xMax X轴最大值
     * @param yMin Y轴最小值
     * @param yMax Y轴最大值
     * @param xLabel X轴标签
     * @param yLabel Y轴标签
     * @return 坐标轴SVG内容
     */
    static std::string createAxes(int width, int height, int margin,
                                 float xMin, float xMax, float yMin, float yMax,
                                 const std::string& xLabel, const std::string& yLabel);
    
    /**
     * @brief 格式化浮点数
     * @param value 数值
     * @param precision 精度
     * @return 格式化后的字符串
     */
    static std::string formatFloat(float value, int precision = 2);
};

} // namespace histogram

#endif // SVG_EXPORTER_HPP
