#include "image.h"

#include <fstream>

#include "exceptions.h"

void Image::Load(const std::string& file_name) {
    std::ifstream ifs(file_name.data(), std::ifstream::binary);
    if (!ifs.is_open()) {
        throw FilesystemError("File '" + file_name + "' cannot be opened for reading");
    }
    Load(ifs);
}

void Image::Load(std::istream& in) {
    in.read(reinterpret_cast<char*>(&bmp_header_), BMP_HEADER_SIZE);
    in.read(reinterpret_cast<char*>(&dib_header_), DIB_HEADER_SIZE);
    ValidateHeader();
    LoadPixels(in);
    if (!in) {
        throw FilesystemError("Loading image failed");
    }
}

void Image::Save(const std::string& file_name) const {
    std::ofstream ofs(file_name.data(), std::ofstream::binary);
    if (!ofs.is_open()) {
        throw FilesystemError("File '" + file_name + "' cannot be opened for writing");
    }
    Save(ofs);
}

void Image::Save(std::ostream& out) const {
    int64_t row_length = GetWidth() * Pixel::NUM_PRIMARY_COLORS + GetPaddingSize();
    std::vector<char> content(bmp_header_.file_size);
    std::copy(reinterpret_cast<const char*>(&bmp_header_),
              reinterpret_cast<const char*>(&bmp_header_) + BMP_HEADER_SIZE, content.data());
    std::copy(reinterpret_cast<const char*>(&dib_header_),
              reinterpret_cast<const char*>(&dib_header_) + DIB_HEADER_SIZE, content.data() + BMP_HEADER_SIZE);
    for (int64_t i = 0; i != GetHeight(); ++i) {
        std::copy(pixels_.begin() + i * GetWidth(), pixels_.begin() + (i + 1) * GetWidth(),
                  reinterpret_cast<Pixel*>(content.data() + BMP_HEADER_SIZE + DIB_HEADER_SIZE + i * row_length));
    }
    out.write(content.data(), bmp_header_.file_size);
    if (!out) {
        throw FilesystemError("Saving image failed");
    }
}

void Image::Crop(int64_t width, int64_t height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Crop failed: width or height is non-positive");
    }
    if (width > GetWidth() && height > GetHeight()) {
        return;
    }
    width = std::min(width, GetWidth());
    height = std::min(height, GetHeight());
    std::vector<Pixel> new_pixels(width * height);
    for (int64_t i = 0; i != height; ++i) {
        std::copy(pixels_.begin() + (i + GetHeight() - height) * GetWidth(),
                  pixels_.begin() + (i + GetHeight() - height) * GetWidth() + width, new_pixels.begin() + i * width);
    }
    SetWidth(width);
    SetHeight(height);
    pixels_.swap(new_pixels);
}

void Image::Swap(Image& other) {
    std::swap(bmp_header_, other.bmp_header_);
    std::swap(dib_header_, other.dib_header_);
    std::swap(pixels_, other.pixels_);
}

int64_t Image::GetPaddingSize() const {
    return (4 - GetWidth() * Pixel::NUM_PRIMARY_COLORS % 4) % 4;
}

void Image::ValidateHeader() const {
    if (bmp_header_.id != BMP_ID) {
        throw IncompatibleFileError("Incompatible file: file isn't BMP");
    }
    if (bmp_header_.pixels_offset != BMP_HEADER_SIZE + DIB_HEADER_SIZE) {
        throw IncompatibleFileError("Incompatible file: wrong pixels data offset");
    }
    if (dib_header_.dib_header_size != DIB_HEADER_SIZE) {
        throw IncompatibleFileError("Incompatible file: wrong DIB header size");
    }
    if (dib_header_.width <= 0 || dib_header_.height <= 0) {
        throw FileCorruptionError("File corruption: width or height has non-positive value");
    }
    if (dib_header_.num_color_planes != COLOR_PLANES_NUMBER) {
        throw FileCorruptionError("File corruption: " + std::to_string(dib_header_.num_color_planes) +
                                  "color planes instead of " + std::to_string(COLOR_PLANES_NUMBER));
    }
    if (dib_header_.bpp != Pixel::NUM_PRIMARY_COLORS * BITS_IN_BYTE) {
        throw IncompatibleFileError("Incompatible file: " + std::to_string(dib_header_.bpp) +
                                    " bits per pixel instead of " +
                                    std::to_string(Pixel::NUM_PRIMARY_COLORS * BITS_IN_BYTE));
    }
    if (dib_header_.compress) {
        throw IncompatibleFileError("Incompatible file: pixel array compression used");
    }
    if (bmp_header_.file_size != bmp_header_.pixels_offset + dib_header_.img_size) {
        throw FileCorruptionError("File corruption: file size is not equal to pixels offset size plus image size");
    }
    if (dib_header_.img_size != (GetWidth() * Pixel::NUM_PRIMARY_COLORS + GetPaddingSize()) * GetHeight()) {
        throw FileCorruptionError("File corruption: image size is not equal to aligned row width multiplied by height");
    }
}

void Image::LoadPixels(std::istream& in) {
    pixels_.resize(GetWidth() * GetHeight());
    int64_t padding = GetPaddingSize();
    for (int64_t i = 0; i != GetHeight(); ++i) {
        in.read(reinterpret_cast<char*>(pixels_.data() + GetWidth() * i), GetWidth() * Pixel::NUM_PRIMARY_COLORS);
        in.ignore(padding);
    }
}

void Image::SetWidth(int64_t width) {
    dib_header_.width = static_cast<int32_t>(width);
    dib_header_.img_size =
        static_cast<uint32_t>((Pixel::NUM_PRIMARY_COLORS * GetWidth() + GetPaddingSize()) * GetHeight());
    bmp_header_.file_size = dib_header_.img_size + bmp_header_.pixels_offset;
}

void Image::SetHeight(int64_t height) {
    dib_header_.height = static_cast<int32_t>(height);
    dib_header_.img_size =
        static_cast<uint32_t>((Pixel::NUM_PRIMARY_COLORS * GetWidth() + GetPaddingSize()) * GetHeight());
    bmp_header_.file_size = dib_header_.img_size + bmp_header_.pixels_offset;
}
