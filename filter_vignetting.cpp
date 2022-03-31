#include "filter_vignetting.h"

#include <cmath>

#include "image.h"

VignettingFilter::VignettingFilter(double alpha) : alpha_(alpha) {
}

Image& VignettingFilter::Apply(Image& image) const {
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            double k1 = static_cast<double>(i) / static_cast<double>(image.GetHeight()) - 0.5;
            double k2 = static_cast<double>(j) / static_cast<double>(image.GetWidth()) - 0.5;
            double k = 1 - 2 * (k1 * k1 + k2 * k2);
            for (size_t x = 0; x != Pixel::NUM_PRIMARY_COLORS; ++x) {
                p.data[x] = static_cast<uint8_t>(double(p.data[x]) * std::pow(k, alpha_));
            }
            image.SetPixel(i, j, p);
        }
    }
    return image;
}
