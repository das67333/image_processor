#pragma once

#include <cstdint>

#include "filter_base.h"

class HexagonalPixellateFilter : public BaseFilter {
public:
    HexagonalPixellateFilter(double a);
    Image& Apply(Image& image) const override;

protected:
    Pixel CalculateHexagonColor(Image& image, int64_t i, int64_t j) const;
    void DrawHexagon(Image& image, int64_t i, int64_t j, Pixel p) const;

    double a_;
};
