#pragma once

#include "filter_base.h"

class BulgeFilter : public BaseFilter {
public:
    BulgeFilter(double alpha);
    Image& Apply(Image& image) const override;

protected:
    double alpha_;
};
