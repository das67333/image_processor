#pragma once

#include "filter_base.h"

class VignettingFilter : public BaseFilter {
public:
    VignettingFilter(double alpha);
    Image& Apply(Image& image) const override;

protected:
    double alpha_;
};
