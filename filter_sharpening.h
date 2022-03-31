#pragma once

#include <cstddef>
#include <utility>

#include "filter_base.h"

class SharpeningFilter : public BaseFilter {
public:
    static const int COEFFICIENT_SELF = 5;
    static const int COEFFICIENT_NEIGHBOR = -1;
    static constexpr size_t NUM_NEIGHBORS = 4;
    static const std::pair<int, int> NEIGHBOR_OFFSETS[NUM_NEIGHBORS];

    Image& Apply(Image& image) const override;
};
