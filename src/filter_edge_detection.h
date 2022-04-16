#pragma once

#include <cstddef>
#include <utility>

#include "filter_grayscale.h"

class EdgeDetectionFilter : public GrayscaleFilter {
public:
    static const int COEFFICIENT_SELF = 4;
    static const int COEFFICIENT_NEIGHBOR = -1;
    static constexpr size_t NUM_NEIGHBORS = 4;
    static const std::pair<int64_t, int64_t> NEIGHBORS_OFFSETS[NUM_NEIGHBORS];

    EdgeDetectionFilter(double threshold);
    Image& Apply(Image& image) const override;

protected:
    double threshold_;
};
