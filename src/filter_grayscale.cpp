#include "filter_grayscale.h"

#include "image.h"

const double GrayscaleFilter::COEFFICIENTS[Pixel::NUM_PRIMARY_COLORS] = {0.114, 0.587, 0.299};

Image& GrayscaleFilter::Apply(Image& image) const {
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            double d = 0;
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                d += p.data[c] * COEFFICIENTS[c];
            }
            uint8_t v = static_cast<uint8_t>(d);
            image.SetPixel(i, j, {v, v, v});
        }
    }
    return image;
}
