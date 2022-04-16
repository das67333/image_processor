#pragma once

#include "filter_base.h"

class CropFilter : public BaseFilter {
public:
    CropFilter(size_t width, size_t height);
    Image& Apply(Image& image) const override;

protected:
    size_t width_;
    size_t height_;
};
