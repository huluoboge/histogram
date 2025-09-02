# Histogram Library

一个功能完整的C++直方图库，支持数据统计、CDF计算、高斯滤波和SVG可视化导出。

## 功能特性

- **直方图统计**: 支持float类型数据，可配置范围和分辨率
- **累计分布函数(CDF)**: 从直方图生成累计分布，计算百分位
- **高斯滤波**: 对直方图进行平滑处理
- **SVG可视化**: 导出直方图、CDF和滤波结果到SVG格式
- **单元测试**: 完整的Google Test测试套件
- **CMake构建**: 现代化的CMake构建系统

## 构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译
make

# 运行测试
make test

# 运行示例
./demo
```

## 使用示例

```cpp
#include "histogram/Histogram.hpp"
#include "histogram/CDF.hpp"
#include "histogram/GaussianFilter.hpp"
#include "histogram/SVGExporter.hpp"

// 创建直方图
histogram::Histogram hist(0.0f, 10.0f, 20);

// 添加数据
hist.addData(5.2f);
hist.addData(3.8f);
// ...

// 计算CDF
histogram::CDF cdf;
cdf.computeFromHistogram(hist);
float median = cdf.getPercentile(50.0f);

// 应用高斯滤波
histogram::GaussianFilter filter(1.0f);
auto filtered = filter.filterCounts(hist.getBinCounts());

// 导出SVG
histogram::SVGExporter::exportHistogram(hist, "output.svg");
```

## 类说明

### Histogram
- `Histogram(float min, float max, size_t resolution)`: 构造函数
- `void addData(float value)`: 添加数据点
- `size_t getBinCount(size_t binIndex)`: 获取bin计数
- `size_t getTotalCount()`: 获取总数据点数

### CDF
- `void computeFromHistogram(const Histogram& hist)`: 从直方图计算CDF
- `float getPercentile(float percentile)`: 获取指定百分位的值
- `float getCumulativeProbability(float value)`: 获取累计概率

### GaussianFilter
- `GaussianFilter(float sigma = 1.0f)`: 构造函数
- `std::vector<float> filterCounts(const std::vector<size_t>& counts)`: 滤波直方图计数

### SVGExporter
- `static void exportHistogram(...)`: 导出直方图到SVG
- `static void exportCDF(...)`: 导出CDF到SVG
- `static void exportFilteredHistogram(...)`: 导出滤波直方图到SVG

## 依赖

- C++17 或更高版本
- Google Test (用于单元测试)
- CMake 3.12 或更高版本

## 许可证

MIT License
