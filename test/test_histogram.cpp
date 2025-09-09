#include <gtest/gtest.h>
#include "histogram/Histogram.hpp"
#include "histogram/CDF.hpp"
#include "histogram/GaussianFilter.hpp"
#include "histogram/SVGExporter.hpp"
#include <vector>
#include <random>
#include <tuple>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

class HistogramTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试输出目录
        fs::create_directories("test_output");
    }
    
    void TearDown() override {
        // 清理测试文件
        // fs::remove_all("test_output");
    }
};

// 测试Histogram基本功能
TEST_F(HistogramTest, BasicFunctionality) {
    histogram::Histogram hist(0.0f, 10.0f, 10);
    
    // 测试空直方图
    EXPECT_EQ(hist.getTotalCount(), 0);
    EXPECT_EQ(hist.getResolution(), 10);
    EXPECT_FLOAT_EQ(hist.getBinWidth(), 1.0f);
    
    // 添加数据
    hist.addData(2.5f);
    hist.addData(3.5f);
    hist.addData(2.8f);
    hist.addData(7.2f);
    
    EXPECT_EQ(hist.getTotalCount(), 4);
    
    // 测试bin计数
    EXPECT_EQ(hist.getBinCount(2), 2); // 2.5和2.8应该在bin 2
    EXPECT_EQ(hist.getBinCount(3), 1); // 3.5应该在bin 3
    EXPECT_EQ(hist.getBinCount(7), 1); // 7.2应该在bin 7
    
    // 测试超出范围的数据
    hist.addData(-1.0f); // 小于min
    hist.addData(11.0f); // 大于max
    EXPECT_EQ(hist.getTotalCount(), 4); // 计数不应增加
}

// 测试CDF计算
TEST_F(HistogramTest, CDFComputation) {
    histogram::Histogram hist(0.0f, 10.0f, 10);
    
    // 添加均匀分布的数据
    for (int i = 0; i < 10; ++i) {
        hist.addData(static_cast<float>(i));
    }
    
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    // 测试累计概率
    EXPECT_FLOAT_EQ(cdf.getCumulativeProbability(0.0f), 0.1f);
    EXPECT_FLOAT_EQ(cdf.getCumulativeProbability(2.0f), 0.3f);
    EXPECT_FLOAT_EQ(cdf.getCumulativeProbability(9.0f), 1.0f);
    
    // 测试百分位
    EXPECT_FLOAT_EQ(cdf.getPercentile(10.0f), 1.0f);
    EXPECT_FLOAT_EQ(cdf.getPercentile(50.0f), 5.0f);
    EXPECT_FLOAT_EQ(cdf.getPercentile(90.0f), 9.0f);
}

// 测试高斯滤波
TEST_F(HistogramTest, GaussianFilter) {
    histogram::GaussianFilter filter(1.0f);
    
    // 创建测试数据（脉冲）
    std::vector<float> testData(10, 0.0f);
    testData[5] = 1.0f;
    
    auto filtered = filter.filter(testData);
    
    // 滤波后应该是一个平滑的高斯分布
    EXPECT_EQ(filtered.size(), testData.size());
    
    // 中心值应该最大
    EXPECT_GT(filtered[5], filtered[4]);
    EXPECT_GT(filtered[5], filtered[6]);
    
    // 对称性检查（使用近似比较，因为浮点数计算有精度差异）
    EXPECT_NEAR(filtered[4], filtered[6], 1e-3f);
    EXPECT_NEAR(filtered[3], filtered[7], 1e-3f);
}

// 测试SVG导出
TEST_F(HistogramTest, SVGExport) {
    histogram::Histogram hist(0.0f, 10.0f, 10);
    
    // 添加一些测试数据
    std::mt19937 gen(42);
    std::normal_distribution<float> dist(5.0f, 2.0f);
    
    for (int i = 0; i < 100; ++i) {
        hist.addData(dist(gen));
    }
    
    // 测试直方图导出
    EXPECT_NO_THROW({
        histogram::SVGExporter::exportHistogram(hist, "test_output/histogram.svg", 800, 600);
    });
    
    // 测试文件是否存在
    EXPECT_TRUE(fs::exists("test_output/histogram.svg"));
    
    // 测试CDF导出
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    EXPECT_NO_THROW({
        histogram::SVGExporter::exportCDF(cdf, "test_output/cdf.svg");
    });
    EXPECT_TRUE(fs::exists("test_output/cdf.svg"));
    
    // 测试滤波直方图导出
    histogram::GaussianFilter gaussianFilter(0.5f);
    auto filtered = gaussianFilter.filterCounts(hist.getBinCounts());
    
    EXPECT_NO_THROW({
        histogram::SVGExporter::exportFilteredHistogram(hist, filtered, "test_output/filtered.svg");
    });
    EXPECT_TRUE(fs::exists("test_output/filtered.svg"));
}

// 测试边界条件
TEST_F(HistogramTest, EdgeCases) {
    // 测试无效参数
    EXPECT_THROW(histogram::Histogram(10.0f, 0.0f, 10), std::invalid_argument);
    EXPECT_THROW(histogram::Histogram(0.0f, 10.0f, 0), std::invalid_argument);
    
    // 测试空直方图的CDF
    histogram::Histogram emptyHist(0.0f, 10.0f, 5);
    histogram::CDF emptyCDF;
    EXPECT_THROW(emptyCDF.computeFromHistogram(emptyHist), std::runtime_error);
    
    // 测试无效百分位
    histogram::Histogram hist(0.0f, 10.0f, 5);
    hist.addData(5.0f);
    histogram::CDF cdf;
    cdf.computeFromHistogram(hist);
    
    EXPECT_THROW(cdf.getPercentile(-1.0f), std::invalid_argument);
    EXPECT_THROW(cdf.getPercentile(101.0f), std::invalid_argument);
}

// 测试高斯滤波参数
TEST_F(HistogramTest, GaussianFilterParameters) {
    EXPECT_THROW(histogram::GaussianFilter(0.0f), std::invalid_argument);
    EXPECT_THROW(histogram::GaussianFilter(-1.0f), std::invalid_argument);
    
    histogram::GaussianFilter filter(1.0f);
    EXPECT_THROW(filter.setSigma(0.0f), std::invalid_argument);
}

// 测试波峰检测功能
TEST_F(HistogramTest, PeakDetection) {
    // 创建多峰值直方图
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    // 生成三个正态分布的混合数据
    std::mt19937 gen(123);
    std::normal_distribution<float> dist1(4.0f, 0.8f);
    std::normal_distribution<float> dist2(10.0f, 1.2f);
    std::normal_distribution<float> dist3(16.0f, 0.9f);
    
    for (int i = 0; i < 400; ++i) hist.addData(dist1(gen));
    for (int i = 0; i < 600; ++i) hist.addData(dist2(gen));
    for (int i = 0; i < 500; ++i) hist.addData(dist3(gen));
    
    // 检测波峰
    auto peaks = hist.findPeaks(0.05f);
    auto peaksInfo = hist.getPeaksInfo(0.05f);
    
    // 应该检测到至少2个波峰
    EXPECT_GE(peaks.size(), 2);
    EXPECT_GE(peaksInfo.size(), 2);
    
    // 检查波峰信息格式
    for (const auto& peak : peaksInfo) {
        size_t index = std::get<0>(peak);
        size_t count = std::get<1>(peak);
        auto range = std::get<2>(peak);
        
        EXPECT_LT(index, hist.getResolution());
        EXPECT_GT(count, 0);
        EXPECT_LT(range.first, range.second);
    }
}

// 测试空直方图的波峰检测
TEST_F(HistogramTest, PeakDetectionEmptyHistogram) {
    histogram::Histogram emptyHist(0.0f, 10.0f, 10);
    
    auto peaks = emptyHist.findPeaks();
    auto peaksInfo = emptyHist.getPeaksInfo();
    
    EXPECT_TRUE(peaks.empty());
    EXPECT_TRUE(peaksInfo.empty());
}

// 测试单峰值直方图
TEST_F(HistogramTest, PeakDetectionSinglePeak) {
    histogram::Histogram hist(0.0f, 10.0f, 20);
    
    std::mt19937 gen(42);
    std::normal_distribution<float> dist(5.0f, 1.0f);
    
    for (int i = 0; i < 1000; ++i) {
        hist.addData(dist(gen));
    }
    
    auto peaks = hist.findPeaks(0.05f);
    EXPECT_GE(peaks.size(), 1);
}

// 测试波峰检测SVG导出
TEST_F(HistogramTest, PeakDetectionSVGExport) {
    histogram::Histogram hist(0.0f, 20.0f, 40);
    
    // 生成多峰值数据
    std::mt19937 gen(789);
    std::normal_distribution<float> dist1(4.0f, 0.8f);
    std::normal_distribution<float> dist2(12.0f, 1.2f);
    std::normal_distribution<float> dist3(16.0f, 0.9f);
    
    for (int i = 0; i < 400; ++i) hist.addData(dist1(gen));
    for (int i = 0; i < 600; ++i) hist.addData(dist2(gen));
    for (int i = 0; i < 500; ++i) hist.addData(dist3(gen));
    
    // 检测波峰
    auto peaksInfo = hist.getPeaksInfo(0.1f);
    
    // 导出带波峰标记的SVG
    EXPECT_NO_THROW({
        histogram::SVGExporter::exportHistogramWithPeaks(hist, peaksInfo, "test_output/histogram_with_peaks.svg", 800, 600, "Histogram with Detected Peaks");
    });
    
    EXPECT_TRUE(fs::exists("test_output/histogram_with_peaks.svg"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
