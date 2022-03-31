#include "filter_hexagonal_pixellate.h"

#include <cmath>

#include "image.h"

HexagonalPixellateFilter::HexagonalPixellateFilter(double a) : a_(a) {
}

Image& HexagonalPixellateFilter::Apply(Image& image) const {
    double k1 = std::sqrt(3) * 0.5;
    int64_t n = static_cast<int64_t>(static_cast<double>(image.GetHeight()) / (a_ * k1)) + 2;
    int64_t m = static_cast<int64_t>(static_cast<double>(image.GetWidth()) / (a_ * 3.)) + 2;
    for (int64_t i = 0; i != n; ++i) {
        double k2 = (i % 2 ? 2 : 0.5);
        for (int64_t j = 0; j != m; ++j) {
            int64_t y = static_cast<int64_t>(a_ * k1 * static_cast<double>(i));
            int64_t x = static_cast<int64_t>(a_ * (k2 + 3. * static_cast<double>(j)));
            Pixel p = CalculateHexagonColor(image, y, x);
            DrawHexagon(image, y, x, p);
        }
    }
    return image;
}

Pixel HexagonalPixellateFilter::CalculateHexagonColor(Image& image, int64_t i, int64_t j) const {
    uint64_t result[Pixel::NUM_PRIMARY_COLORS] = {0};
    uint64_t n = 0;
    double k1 = std::sqrt(3) * 0.5, k2 = 1 / std::sqrt(3);
    int64_t h_max = static_cast<int64_t>(std::ceil(a_ * k1));
    for (int64_t h = 0; h <= h_max; ++h) {
        int64_t w = static_cast<int64_t>(a_ - static_cast<double>(h) * k2) + 1;
        for (int64_t x = j - w; x != j + w; ++x) {
            int64_t y = std::clamp(i - h - 1, 0l, image.GetHeight() - 1);
            int64_t z = std::clamp(x, 0l, image.GetWidth() - 1);
            Pixel p = image.GetPixel(y, z);
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                result[c] += p.data[c];
            }
            ++n;
        }
        for (int64_t x = j - w; x != j + w; ++x) {
            int64_t y = std::clamp(i + h, 0l, image.GetHeight() - 1);
            int64_t z = std::clamp(x, 0l, image.GetWidth() - 1);
            Pixel p = image.GetPixel(y, z);
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                result[c] += p.data[c];
            }
            ++n;
        }
    }
    Pixel p;
    for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
        p.data[c] = static_cast<uint8_t>(result[c] / n);
    }
    return p;
}

void HexagonalPixellateFilter::DrawHexagon(Image& image, int64_t i, int64_t j, Pixel p) const {
    double k1 = std::sqrt(3) * 0.5, k2 = 1 / std::sqrt(3);
    int64_t h_max = static_cast<int64_t>(std::ceil(a_ * k1));
    for (int64_t h = 0; h <= h_max; ++h) {
        int64_t w = static_cast<int64_t>(a_ - static_cast<double>(h) * k2) + 1;
        for (int64_t x = j - w; x != j + w; ++x) {
            int64_t y = i - h - 1;
            if (0 <= y && y < image.GetHeight() && 0 <= x && x < image.GetWidth()) {
                image.SetPixel(y, x, p);
            }
        }
        for (int64_t x = j - w; x != j + w; ++x) {
            int64_t y = i + h;
            if (0 <= y && y < image.GetHeight() && 0 <= x && x < image.GetWidth()) {
                image.SetPixel(y, x, p);
            }
        }
    }
}
