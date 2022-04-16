#pragma once

#include <vector>

#include "filter_base.h"

class GaussianBlurFilter : public BaseFilter {
public:
    GaussianBlurFilter(double sigma, double precision);
    Image& Apply(Image& image) const override;

protected:
    std::vector<double> CalculateWeights() const;

    double sigma_;
    double precision_;
};
