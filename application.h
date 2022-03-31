#pragma once

#include "filter_pipeline_factory.h"

class Application {
public:
    void Configure();
    void Run(int argc, const char* argv[]);

protected:
    FilterPipelineFactory fpf_;
};

class BaseFilter;
struct FilterDescriptor;

namespace FilterFactories {
BaseFilter* CreateCropFilter(const FilterDescriptor& fd);
BaseFilter* CreateGrayscaleFilter(const FilterDescriptor& fd);
BaseFilter* CreateNegativeFilter(const FilterDescriptor& fd);
BaseFilter* CreateSharpeningFilter(const FilterDescriptor& fd);
BaseFilter* CreateEdgeDetectionFilter(const FilterDescriptor& fd);
BaseFilter* CreateGaussianBlurFilter(const FilterDescriptor& fd);
BaseFilter* CreateHexagonalPixellateFilter(const FilterDescriptor& fd);
BaseFilter* CreateCrystallizeFilter(const FilterDescriptor& fd);
BaseFilter* CreateVignettingFilter(const FilterDescriptor& fd);
BaseFilter* CreateBulgeFilter(const FilterDescriptor& fd);
BaseFilter* CreateGridFilter(const FilterDescriptor& fd);
};  // namespace FilterFactories
