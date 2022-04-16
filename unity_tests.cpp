#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#include "catch2/catch.hpp"

#include <array>
#include <sstream>
#include <vector>

#include "cmd_arg_parser.h"
#include "filter_pipeline_factory.h"
#include "image.h"
#include "filter_crop.h"
#include "filter_grayscale.h"
#include "filter_negative.h"

const std::array<uint8_t, 70> VALID_DATA = {
    0x42, 0x4D, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};

bool operator==(Pixel a, Pixel b) {
    for (size_t c = 0; c != Pixel::NUM_PRIMARY_COLORS; ++c) {
        if (a.data[c] != b.data[c]) {
            return false;
        }
    }
    return true;
}

template <size_t N>
Image CreateImage(const std::array<uint8_t, N>& arr) {
    Image image;
    std::stringstream s;
    for (auto i : arr) {
        s << i;
    }
    image.Load(s);
    return image;
}

TEST_CASE("CmdArgParser") {
    CmdArgParser parser;
    std::vector<const char*> argv{"path", "ifile", "ofile", "-f1", "1", "2.0", "-f2", nullptr};
    parser.Parse(static_cast<int>(argv.size()) - 1, argv.data());
    REQUIRE(parser.GetInputFileName() == "ifile");
    REQUIRE(parser.GetOutputFileName() == "ofile");
    auto descrs = parser.GetFilterDescriptors();
    REQUIRE(descrs.size() == 2);
    REQUIRE(descrs[0].name == "f1");
    REQUIRE((descrs[0].params == std::vector<std::string>{"1", "2.0"}));
    REQUIRE(descrs[1].name == "f2");
    REQUIRE(descrs[1].params.empty());
}

TEST_CASE("Image") {
    Image image;
    try {
        image = CreateImage(VALID_DATA);
    } catch (...) {
        REQUIRE(false);
    }
    Image copy = image;

    REQUIRE(image.GetWidth() == 2);
    REQUIRE(image.GetHeight() == 2);

    REQUIRE((image.GetPixel(0, 0) == Pixel{0x00, 0x00, 0xFF}));
    REQUIRE((image.GetPixel(0, 1) == Pixel{0xFF, 0xFF, 0xFF}));
    REQUIRE((image.GetPixel(1, 0) == Pixel{0xFF, 0x00, 0x00}));
    REQUIRE((image.GetPixel(1, 1) == Pixel{0x00, 0xFF, 0x00}));

    image.SetPixel(0, 1, {0x88, 0x77, 0x66});
    REQUIRE((image.GetPixel(0, 1) == Pixel{0x88, 0x77, 0x66}));
    image.Swap(copy);
    REQUIRE((image.GetPixel(0, 1) == Pixel{0xFF, 0xFF, 0xFF}));
    REQUIRE((copy.GetPixel(0, 1) == Pixel{0x88, 0x77, 0x66}));

    copy = image;
    std::stringstream s;
    image.Save(s);
    image.Load(s);
    REQUIRE(image.GetWidth() == copy.GetWidth());
    REQUIRE(image.GetHeight() == copy.GetHeight());
    for (int64_t i = 0; i != image.GetHeight(); ++i) {
        for (int64_t j = 0; j != image.GetWidth(); ++j) {
            REQUIRE(image.GetPixel(i, j) == copy.GetPixel(i, j));
        }
    }
}

TEST_CASE("Crop") {
    Image image = CreateImage(VALID_DATA);

    CropFilter(1, 5).Apply(image);
    REQUIRE(image.GetWidth() == 1);
    REQUIRE(image.GetHeight() == 2);
    CropFilter(5, 1).Apply(image);
    REQUIRE(image.GetWidth() == 1);
    REQUIRE(image.GetHeight() == 1);
    REQUIRE((image.GetPixel(0, 0) == Pixel{0xFF, 0x00, 0x00}));
}

TEST_CASE("Grayscale") {
    Image image = CreateImage(VALID_DATA);

    GrayscaleFilter().Apply(image);
    REQUIRE((image.GetPixel(0, 0) == Pixel{0x4C, 0x4C, 0x4C}));
    REQUIRE((image.GetPixel(0, 1) == Pixel{0xFF, 0xFF, 0xFF}));
    REQUIRE((image.GetPixel(1, 0) == Pixel{0x1D, 0x1D, 0x1D}));
    REQUIRE((image.GetPixel(1, 1) == Pixel{0x95, 0x95, 0x95}));
}

TEST_CASE("Negative") {
    Image image = CreateImage(VALID_DATA);

    NegativeFilter().Apply(image);
    REQUIRE((image.GetPixel(0, 0) == Pixel{0xFF, 0xFF, 0x00}));
    REQUIRE((image.GetPixel(0, 1) == Pixel{0x00, 0x00, 0x00}));
    REQUIRE((image.GetPixel(1, 0) == Pixel{0x00, 0xFF, 0xFF}));
    REQUIRE((image.GetPixel(1, 1) == Pixel{0xFF, 0x00, 0xFF}));
}
