#include "filter_bulge.h"

#include <cmath>

#include "image.h"

BulgeFilter::BulgeFilter(double alpha) : alpha_(alpha) {
}

Image& BulgeFilter::Apply(Image& image) const {
    Image new_image = image;
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            double id = i, jd = j, hd = image.GetHeight(), wd = image.GetWidth();
            double k1 = id / hd * 2 - 1;
            double k2 = jd / wd * 2 - 1;
            double r = std::sqrt(k1 * k1 + k2 * k2);
            double k = alpha_ * r + 1 - alpha_;
            k1 = (k1 * k + 1) * 0.5;
            k2 = (k2 * k + 1) * 0.5;
            int64_t l1 = std::clamp(static_cast<int64_t>(k1 * image.GetHeight()), 0l, image.GetHeight() - 1);
            int64_t l2 = std::clamp(static_cast<int64_t>(k2 * image.GetWidth()), 0l, image.GetWidth() - 1);
            Pixel p = image.GetPixel(l1, l2);
            new_image.SetPixel(i, j, p);
        }
    }
    image.Swap(new_image);
    return image;
}
