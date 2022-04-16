#pragma once

#include <vector>

class Image;
class BaseFilter;

class FilterPipeline {
public:
    FilterPipeline() = default;
    FilterPipeline& operator=(const FilterPipeline& another) = delete;
    FilterPipeline(const FilterPipeline& another) = delete;

    FilterPipeline(FilterPipeline&& another);
    FilterPipeline& operator=(FilterPipeline&& another);
    ~FilterPipeline();

    void AddFilter(BaseFilter* filter);
    void ApplyAll(Image& image) const;

protected:
    std::vector<BaseFilter*> filters_;
};
