#include "filter_grid.h"

#include <cmath>

#include "image.h"

const Pixel GridFilter::COLOR = {0x00, 0xFF, 0x00};

GridFilter::GridFilter(int64_t n, int64_t width) : step_(n), width_(width) {
}

Image& GridFilter::Apply(Image& image) const {
    int64_t i0 = image.GetHeight() / 2 % step_ - width_ / 2;
    int64_t j0 = image.GetWidth() / 2 % step_ - width_ / 2;
    for (int64_t i = 0; i < image.GetHeight(); ++i) {
        if ((i - i0 + step_) % step_ < width_) {
            for (int64_t j = 0; j != image.GetWidth(); ++j) {
                image.SetPixel(i, j, COLOR);
            }
        } else {
            for (int64_t j = j0; j < image.GetWidth(); j += step_) {
                for (int64_t k = 0; k != width_; ++k) {
                    if (j + k >= 0 && j + k < image.GetWidth()) {
                        image.SetPixel(i, j + k, COLOR);
                    }
                }
            }
        }
    }
    return image;
}
