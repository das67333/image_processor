#pragma once

#include <cstdint>
#include <vector>

#include "filter_base.h"

class CrystallizeFilter : public BaseFilter {
public:
    struct Crystal {
        int64_t y = 0;
        int64_t x = 0;
        uint64_t color[Pixel::NUM_PRIMARY_COLORS] = {0};
        uint64_t n = 0;
    };

    CrystallizeFilter(int64_t n);
    Image& Apply(Image& image) const override;

protected:
    std::vector<Crystal> CreateCrystals(int64_t n, int64_t m, int64_t a) const;

    int64_t n_;
};
