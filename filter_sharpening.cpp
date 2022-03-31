#include "filter_sharpening.h"

#include "image.h"

const std::pair<int, int> SharpeningFilter::NEIGHBOR_OFFSETS[NUM_NEIGHBORS] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

Image& SharpeningFilter::Apply(Image& image) const {
    Image new_image = image;
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            Pixel neighbors[NUM_NEIGHBORS];
            for (size_t k = 0; k != NUM_NEIGHBORS; ++k) {
                auto [oi, oj] = NEIGHBOR_OFFSETS[k];
                int64_t y = std::clamp(i + oi, 0l, image.GetHeight() - 1);
                int64_t x = std::clamp(j + oj, 0l, image.GetWidth() - 1);
                neighbors[k] = image.GetPixel(y, x);
            }
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                int v = p.data[c] * COEFFICIENT_SELF;
                for (size_t k = 0; k != NUM_NEIGHBORS; ++k) {
                    v += neighbors[k].data[c] * COEFFICIENT_NEIGHBOR;
                }
                p.data[c] = static_cast<uint8_t>(std::clamp(v, 0, UINT8_MAX));
            }
            new_image.SetPixel(i, j, p);
        }
    }
    image.Swap(new_image);
    return image;
}
