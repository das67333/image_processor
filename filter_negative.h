#pragma once

#include "filter_base.h"

class NegativeFilter : public BaseFilter {
public:
    Image& Apply(Image& image) const override;
};
