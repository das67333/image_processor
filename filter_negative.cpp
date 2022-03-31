#include "filter_negative.h"

#include "image.h"

Image& NegativeFilter::Apply(Image& image) const {
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            for (size_t x = 0; x != Pixel::NUM_PRIMARY_COLORS; ++x) {
                p.data[x] = UINT8_MAX - p.data[x];
            }
            image.SetPixel(i, j, p);
        }
    }
    return image;
}
