#pragma once

#include "pixel.h"

class Image;

class BaseFilter {
public:
    static constexpr size_t NUM_PRIMARY_COLORS = 3;

    virtual ~BaseFilter() = default;
    virtual Image& Apply(Image& image) const = 0;
};
