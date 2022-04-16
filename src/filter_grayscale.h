#pragma once

#include "filter_base.h"

class GrayscaleFilter : public BaseFilter {
public:
    static const double COEFFICIENTS[Pixel::NUM_PRIMARY_COLORS];

    Image& Apply(Image& image) const override;
};
