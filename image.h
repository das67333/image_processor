#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pixel.h"

class Image {
public:
    struct BmpHeader {
        uint16_t id;
        uint32_t file_size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t pixels_offset;
    } __attribute__((packed));

    struct DibHeader {
        uint32_t dib_header_size;
        int32_t width;
        int32_t height;
        uint16_t num_color_planes;
        uint16_t bpp;
        uint32_t compress;
        uint32_t img_size;
        int32_t hor_res;
        int32_t ver_res;
        uint32_t num_colors;
        uint32_t imp_color_used;

    } __attribute__((packed));

    static const int64_t BITS_IN_BYTE = 8;
    static const int64_t BMP_HEADER_SIZE = 14;
    static const int64_t DIB_HEADER_SIZE = 40;
    static const uint16_t BMP_ID = 0x4D42;
    static const uint16_t COLOR_PLANES_NUMBER = 1;

    void Load(const std::string& file_name);
    void Load(std::istream& in);
    void Save(const std::string& file_name) const;
    void Save(std::ostream& out) const;
    void Crop(int64_t width, int64_t height);
    int64_t GetWidth() const {
        return dib_header_.width;
    }
    int64_t GetHeight() const {
        return dib_header_.height;
    }
    Pixel GetPixel(int64_t i, int64_t j) const {
        return pixels_[GetWidth() * i + j];
    }
    void SetPixel(int64_t i, int64_t j, Pixel pixel) {
        pixels_[GetWidth() * i + j] = pixel;
    }
    void Swap(Image& other);

protected:
    int64_t GetPaddingSize() const;
    void ValidateHeader() const;
    void LoadPixels(std::istream& in);
    void SetWidth(int64_t width);
    void SetHeight(int64_t height);

    BmpHeader bmp_header_;
    DibHeader dib_header_;
    std::vector<Pixel> pixels_;
};
