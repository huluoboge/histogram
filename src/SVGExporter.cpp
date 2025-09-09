#include "histogram/SVGExporter.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

namespace histogram {

void SVGExporter::exportHistogram(const Histogram& hist, 
                                 const std::string& filename,
                                 int width, 
                                 int height,
                                 const std::string& title) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int margin = 80;
    int chartWidth = width - 2 * margin;
    int chartHeight = height - 2 * margin;
    
    const auto& binCounts = hist.getBinCounts();
    if (binCounts.empty()) {
        throw std::runtime_error("Histogram has no data");
    }
    
    // 找到最大计数值
    size_t maxCount = *std::max_element(binCounts.begin(), binCounts.end());
    if (maxCount == 0) maxCount = 1;
    
    file << createSVGHeader(width, height, title);
    
    // 创建坐标轴
    file << createAxes(chartWidth, chartHeight, margin,
                      hist.getMin(), hist.getMax(), 0, maxCount,
                      "Value", "Count");
    
    // 绘制直方图柱子
    float binWidth = static_cast<float>(chartWidth) / hist.getResolution();
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        if (binCounts[i] > 0) {
            float x = margin + i * binWidth;
            float barHeight = (static_cast<float>(binCounts[i]) / maxCount) * chartHeight;
            float y = margin + chartHeight - barHeight;
            
            file << "<rect x=\"" << x << "\" y=\"" << y << "\" "
                 << "width=\"" << binWidth - 1 << "\" height=\"" << barHeight << "\" "
                 << "fill=\"steelblue\" stroke=\"none\"/>\n";
        }
    }
    
    file << createSVGFooter();
}

void SVGExporter::exportCDF(const CDF& cdf,
                           const std::string& filename,
                           int width,
                           int height,
                           const std::string& title) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int margin = 80;
    int chartWidth = width - 2 * margin;
    int chartHeight = height - 2 * margin;
    
    const auto& cdfValues = cdf.getCDFValues();
    if (cdfValues.empty()) {
        throw std::runtime_error("CDF has no data");
    }
    
    file << createSVGHeader(width, height, title);
    
    // 创建坐标轴
    file << createAxes(chartWidth, chartHeight, margin,
                      0, 100, 0, 1.0f,
                      "Percentile (%)", "Cumulative Probability");
    
    // 绘制CDF曲线
    file << "<polyline points=\"";
    for (size_t i = 0; i < cdfValues.size(); ++i) {
        float x = margin + (static_cast<float>(i) / (cdfValues.size() - 1)) * chartWidth;
        float y = margin + chartHeight - (cdfValues[i] * chartHeight);
        file << x << "," << y << " ";
    }
    file << "\" fill=\"none\" stroke=\"red\" stroke-width=\"2\"/>\n";
    
    file << createSVGFooter();
}

void SVGExporter::exportFilteredHistogram(const Histogram& hist,
                                         const std::vector<float>& filtered,
                                         const std::string& filename,
                                         int width,
                                         int height,
                                         const std::string& title) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int margin = 80;
    int chartWidth = width - 2 * margin;
    int chartHeight = height - 2 * margin;
    
    if (filtered.empty() || filtered.size() != hist.getResolution()) {
        throw std::runtime_error("Filtered data size mismatch");
    }
    
    // 找到最大计数值
    float maxCount = *std::max_element(filtered.begin(), filtered.end());
    if (maxCount == 0) maxCount = 1.0f;
    
    file << createSVGHeader(width, height, title);
    
    // 创建坐标轴
    file << createAxes(chartWidth, chartHeight, margin,
                      hist.getMin(), hist.getMax(), 0, maxCount,
                      "Value", "Count");
    
    // 绘制原始直方图（半透明）
    const auto& binCounts = hist.getBinCounts();
    float binWidth = static_cast<float>(chartWidth) / hist.getResolution();
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        if (binCounts[i] > 0) {
            float x = margin + i * binWidth;
            float barHeight = (static_cast<float>(binCounts[i]) / maxCount) * chartHeight;
            float y = margin + chartHeight - barHeight;
            
            file << "<rect x=\"" << x << "\" y=\"" << y << "\" "
                 << "width=\"" << binWidth - 1 << "\" height=\"" << barHeight << "\" "
                 << "fill=\"lightblue\" stroke=\"none\" opacity=\"0.5\"/>\n";
        }
    }
    
    // 绘制滤波后的曲线
    file << "<polyline points=\"";
    for (size_t i = 0; i < filtered.size(); ++i) {
        float x = margin + (static_cast<float>(i) / (filtered.size() - 1)) * chartWidth;
        float y = margin + chartHeight - (filtered[i] / maxCount * chartHeight);
        file << x << "," << y << " ";
    }
    file << "\" fill=\"none\" stroke=\"darkred\" stroke-width=\"2\"/>\n";
    
    file << createSVGFooter();
}

std::string SVGExporter::createSVGHeader(int width, int height, const std::string& title) {
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<svg width=\"" << width << "\" height=\"" << height << "\" "
       << "xmlns=\"http://www.w3.org/2000/svg\">\n"
       << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n"
       << "<text x=\"" << width/2 << "\" y=\"30\" "
       << "text-anchor=\"middle\" font-size=\"20\" font-family=\"Arial\">"
       << title << "</text>\n";
    return ss.str();
}

std::string SVGExporter::createSVGFooter() {
    return "</svg>";
}

std::string SVGExporter::createAxes(int width, int height, int margin,
                                   float xMin, float xMax, float yMin, float yMax,
                                   const std::string& xLabel, const std::string& yLabel) {
    std::stringstream ss;
    
    // 绘制坐标轴
    ss << "<line x1=\"" << margin << "\" y1=\"" << margin + height << "\" "
       << "x2=\"" << margin + width << "\" y2=\"" << margin + height << "\" "
       << "stroke=\"black\" stroke-width=\"2\"/>\n";
    
    ss << "<line x1=\"" << margin << "\" y1=\"" << margin << "\" "
       << "x2=\"" << margin << "\" y2=\"" << margin + height << "\" "
       << "stroke=\"black\" stroke-width=\"2\"/>\n";
    
    // X轴标签
    ss << "<text x=\"" << margin + width/2 << "\" y=\"" << margin + height + 40 << "\" "
       << "text-anchor=\"middle\" font-size=\"14\" font-family=\"Arial\">"
       << xLabel << "</text>\n";
    
    // Y轴标签（旋转90度）
    ss << "<text x=\"20\" y=\"" << margin + height/2 << "\" "
       << "text-anchor=\"middle\" font-size=\"14\" font-family=\"Arial\" "
       << "transform=\"rotate(-90, 20, " << margin + height/2 << ")\">"
       << yLabel << "</text>\n";
    
    // X轴刻度
    for (int i = 0; i <= 5; ++i) {
        float value = xMin + (xMax - xMin) * i / 5;
        float x = margin + (static_cast<float>(i) / 5) * width;
        ss << "<line x1=\"" << x << "\" y1=\"" << margin + height << "\" "
           << "x2=\"" << x << "\" y2=\"" << margin + height + 5 << "\" "
           << "stroke=\"black\"/>\n";
        ss << "<text x=\"" << x << "\" y=\"" << margin + height + 20 << "\" "
           << "text-anchor=\"middle\" font-size=\"12\" font-family=\"Arial\">"
           << formatFloat(value) << "</text>\n";
    }
    
    // Y轴刻度
    for (int i = 0; i <= 5; ++i) {
        float value = yMin + (yMax - yMin) * i / 5;
        float y = margin + height - (static_cast<float>(i) / 5) * height;
        ss << "<line x1=\"" << margin << "\" y1=\"" << y << "\" "
           << "x2=\"" << margin - 5 << "\" y2=\"" << y << "\" "
           << "stroke=\"black\"/>\n";
        ss << "<text x=\"" << margin - 10 << "\" y=\"" << y + 4 << "\" "
           << "text-anchor=\"end\" font-size=\"12\" font-family=\"Arial\">"
           << formatFloat(value) << "</text>\n";
    }
    
    return ss.str();
}

std::string SVGExporter::formatFloat(float value, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

void SVGExporter::exportHistogramWithPeaks(const Histogram& hist,
                                          const std::vector<std::tuple<size_t, size_t, std::pair<float, float>>>& peaksInfo,
                                          const std::string& filename,
                                          int width,
                                          int height,
                                          const std::string& title) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int margin = 80;
    int chartWidth = width - 2 * margin;
    int chartHeight = height - 2 * margin;
    
    const auto& binCounts = hist.getBinCounts();
    if (binCounts.empty()) {
        throw std::runtime_error("Histogram has no data");
    }
    
    // 找到最大计数值
    size_t maxCount = *std::max_element(binCounts.begin(), binCounts.end());
    if (maxCount == 0) maxCount = 1;
    
    file << createSVGHeader(width, height, title);
    
    // 创建坐标轴
    file << createAxes(chartWidth, chartHeight, margin,
                      hist.getMin(), hist.getMax(), 0, maxCount,
                      "Value", "Count");
    
    // 绘制直方图柱子
    float binWidth = static_cast<float>(chartWidth) / hist.getResolution();
    for (size_t i = 0; i < hist.getResolution(); ++i) {
        if (binCounts[i] > 0) {
            float x = margin + i * binWidth;
            float barHeight = (static_cast<float>(binCounts[i]) / maxCount) * chartHeight;
            float y = margin + chartHeight - barHeight;
            
            file << "<rect x=\"" << x << "\" y=\"" << y << "\" "
                 << "width=\"" << binWidth - 1 << "\" height=\"" << barHeight << "\" "
                 << "fill=\"steelblue\" stroke=\"none\"/>\n";
        }
    }
    
    // 标记波峰
    if (!peaksInfo.empty()) {
        for (const auto& peak : peaksInfo) {
            size_t index = std::get<0>(peak);
            size_t count = std::get<1>(peak);
            auto range = std::get<2>(peak);
            
            float x = margin + index * binWidth + binWidth / 2;
            float barHeight = (static_cast<float>(count) / maxCount) * chartHeight;
            float y = margin + chartHeight - barHeight;
            
            // 绘制波峰标记（红色三角形）
            float markerSize = 10.0f;
            file << "<polygon points=\""
                 << x << "," << y - markerSize << " "
                 << x - markerSize/2 << "," << y << " "
                 << x + markerSize/2 << "," << y
                 << "\" fill=\"red\" stroke=\"darkred\" stroke-width=\"1\"/>\n";
            
            // 添加波峰信息文本
            float center = (range.first + range.second) / 2.0f;
            file << "<text x=\"" << x << "\" y=\"" << y - markerSize - 5 << "\" "
                 << "text-anchor=\"middle\" font-size=\"12\" font-family=\"Arial\" "
                 << "fill=\"darkred\" font-weight=\"bold\">"
                 << "Peak: " << formatFloat(center, 2) << "</text>\n";
        }
        
        // 添加图例
        file << "<text x=\"" << width - margin << "\" y=\"" << margin - 20 << "\" "
             << "text-anchor=\"end\" font-size=\"14\" font-family=\"Arial\" "
             << "fill=\"darkred\" font-weight=\"bold\">"
             << "Detected Peaks: " << peaksInfo.size() << "</text>\n";
    }
    
    file << createSVGFooter();
}

} // namespace histogram
