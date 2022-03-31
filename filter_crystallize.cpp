#include "filter_crystallize.h"

#include <cmath>
#include <random>

#include "image.h"

CrystallizeFilter::CrystallizeFilter(int64_t n) : n_(n) {
}

Image& CrystallizeFilter::Apply(Image& image) const {
    if (image.GetHeight() * image.GetWidth() < n_) {
        return image;
    }
    int64_t a = static_cast<int64_t>(std::sqrt(image.GetHeight() * image.GetWidth() / n_));
    int64_t n = std::max(image.GetHeight() / a, 1l);
    int64_t m = std::max(image.GetWidth() / a, 1l);
    auto dist = [](int64_t ay, int64_t ax, int64_t by, int64_t bx) {
        return (ay - by) * (ay - by) + (ax - bx) * (ax - bx);
    };
    std::vector<Crystal> crystals = CreateCrystals(n, m, a);
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            int64_t dist_min = INT64_MAX;
            Crystal* cr = nullptr;
            for (int64_t on = -1; on <= 1; ++on) {
                for (int64_t om = -1; om <= 1; ++om) {
                    int64_t nn = std::clamp(i / a + on, 0l, n - 1);
                    int64_t mm = std::clamp(j / a + om, 0l, m - 1);
                    Crystal& cr0 = crystals[nn * m + mm];
                    int64_t d = dist(i, j, cr0.y, cr0.x);
                    if (d < dist_min) {
                        dist_min = d;
                        cr = &cr0;
                    }
                }
            }
            ++cr->n;
            Pixel p = image.GetPixel(i, j);
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                cr->color[c] += p.data[c];
            }
        }
    }
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            int64_t dist_min = INT64_MAX;
            Crystal* cr = nullptr;
            for (int64_t on = -1; on <= 1; ++on) {
                for (int64_t om = -1; om <= 1; ++om) {
                    int64_t nn = std::clamp(i / a + on, 0l, n - 1);
                    int64_t mm = std::clamp(j / a + om, 0l, m - 1);
                    Crystal& cr0 = crystals[nn * m + mm];
                    int64_t d = dist(i, j, cr0.y, cr0.x);
                    if (d < dist_min) {
                        dist_min = d;
                        cr = &cr0;
                    }
                }
            }
            Pixel p;
            for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
                p.data[c] = static_cast<uint8_t>(cr->color[c] / cr->n);
            }
            image.SetPixel(i, j, p);
        }
    }
    return image;
}

std::vector<CrystallizeFilter::Crystal> CrystallizeFilter::CreateCrystals(int64_t n, int64_t m, int64_t a) const {
    std::vector<Crystal> result(n * m);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dist(0, a - 1);
    for (int64_t i = 0; i != n; ++i) {
        for (int64_t j = 0; j != m; ++j) {
            Crystal& c = result[i * m + j];
            c.y = i * a + dist(gen);
            c.x = j * a + dist(gen);
        }
    }
    return result;
}
