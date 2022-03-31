#include "filter_pipeline.h"

#include "image.h"
#include "filter_base.h"

FilterPipeline::FilterPipeline(FilterPipeline&& fp) : filters_(std::move(fp.filters_)) {
    fp.filters_.clear();
}

FilterPipeline& FilterPipeline::operator=(FilterPipeline&& another) {
    if (this != &another) {
        filters_.swap(another.filters_);
    }
    return *this;
}

FilterPipeline::~FilterPipeline() {
    for (BaseFilter* f : filters_) {
        delete f;
    }
}

void FilterPipeline::AddFilter(BaseFilter* filter) {
    filters_.push_back(filter);
}

void FilterPipeline::ApplyAll(Image& image) const {
    for (BaseFilter* filter : filters_) {
        filter->Apply(image);
    }
}
