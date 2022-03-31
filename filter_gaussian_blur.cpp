#include "filter_gaussian_blur.h"

#include <cmath>
#include <numbers>

#include "image.h"

GaussianBlurFilter::GaussianBlurFilter(double sigma, double precision) : sigma_(sigma), precision_(precision) {
}

Image& GaussianBlurFilter::Apply(Image& image) const {
    std::vector<double> weights = CalculateWeights();
    int64_t k = (weights.size() - 1) / 2;
    int64_t ws = weights.size();
    Image new_image = image;
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            double value[Pixel::NUM_PRIMARY_COLORS] = {0};
            for (int64_t offset = 0; offset != ws; ++offset) {
                int64_t t = std::clamp(j + offset - k, 0l, image.GetWidth() - 1);
                Pixel n = image.GetPixel(i, t);
                for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                    value[c] += n.data[c] * weights[offset];
                }
            }
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                value[c] = std::min<double>(value[c], UINT8_MAX);
                p.data[c] = static_cast<uint8_t>(value[c]);
            }
            new_image.SetPixel(i, j, p);
        }
    }
    image.Swap(new_image);
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            Pixel p = image.GetPixel(i, j);
            double value[Pixel::NUM_PRIMARY_COLORS] = {0};
            for (int64_t offset = 0; offset != ws; ++offset) {
                int64_t t = std::clamp(i + offset - k, 0l, image.GetHeight() - 1);
                Pixel n = image.GetPixel(t, j);
                for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                    value[c] += n.data[c] * weights[offset];
                }
            }
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                value[c] = std::min<double>(value[c], UINT8_MAX);
                p.data[c] = static_cast<uint8_t>(value[c]);
            }
            new_image.SetPixel(i, j, p);
        }
    }
    image.Swap(new_image);
    return image;
}

std::vector<double> GaussianBlurFilter::CalculateWeights() const {
    double x = 0, sum = 0, k = std::sqrt(2 * std::numbers::pi_v<double>) * sigma_;
    std::vector<double> v;
    do {
        double y = x / sigma_;
        double w = 1 / (k * std::exp(y * y * 0.5));
        v.push_back(w);
        sum += w * (x == 0 ? 1 : 2);
        x += 1;
    } while (sum + precision_ < 1);
    for (auto& i : v) {
        i /= sum;
    }
    std::vector<double> result;
    auto it = --v.end();
    while (it != v.begin()) {
        result.push_back(*it);
        --it;
    }
    while (it != v.end()) {
        result.push_back(*it);
        ++it;
    }
    return result;
}
