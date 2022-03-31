#include "filter_edge_detection.h"

#include "image.h"

const std::pair<int64_t, int64_t> EdgeDetectionFilter::NEIGHBORS_OFFSETS[NUM_NEIGHBORS] = {
    {-1, 0}, {1, 0}, {0, 1}, {0, -1}};

EdgeDetectionFilter::EdgeDetectionFilter(double threshold) : threshold_(threshold) {
}

Image& EdgeDetectionFilter::Apply(Image& image) const {
    GrayscaleFilter::Apply(image);
    Image new_image = image;
    int threshold = static_cast<int>(UINT8_MAX * threshold_);
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            int v = p.data[0] * COEFFICIENT_SELF;
            for (size_t k = 0; k != NUM_NEIGHBORS; ++k) {
                auto [oi, oj] = NEIGHBORS_OFFSETS[k];
                int64_t y = std::clamp(i + oi, 0l, image.GetHeight() - 1);
                int64_t x = std::clamp(j + oj, 0l, image.GetWidth() - 1);
                v += image.GetPixel(y, x).data[0] * COEFFICIENT_NEIGHBOR;
            }
            uint8_t v8 = v > threshold ? UINT8_MAX : 0;
            new_image.SetPixel(i, j, {v8, v8, v8});
        }
    }
    image.Swap(new_image);
    return image;
}
