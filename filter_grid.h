#pragma once

#include "filter_base.h"

class GridFilter : public BaseFilter {
public:
    static const Pixel COLOR;

    GridFilter(int64_t n, int64_t width);
    Image& Apply(Image& image) const override;

protected:
    int64_t step_;
    int64_t width_;
};
