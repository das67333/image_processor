#include "filter_crop.h"

#include "image.h"

CropFilter::CropFilter(size_t width, size_t height) : width_(width), height_(height) {
}

Image& CropFilter::Apply(Image& image) const {
    image.Crop(width_, height_);
    return image;
}
