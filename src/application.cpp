#include "application.h"

#include <iostream>

#include "cmd_arg_parser.h"
#include "filter_pipeline.h"
#include "exceptions.h"
#include "image.h"
#include "filter_crop.h"
#include "filter_grayscale.h"
#include "filter_negative.h"
#include "filter_sharpening.h"
#include "filter_edge_detection.h"
#include "filter_gaussian_blur.h"
#include "filter_hexagonal_pixellate.h"
#include "filter_crystallize.h"
#include "filter_vignetting.h"
#include "filter_bulge.h"
#include "filter_grid.h"

void Application::Configure() {
    fpf_.Configure({{"crop", FilterFactories::CreateCropFilter},
                    {"gs", FilterFactories::CreateGrayscaleFilter},
                    {"neg", FilterFactories::CreateNegativeFilter},
                    {"sharp", FilterFactories::CreateSharpeningFilter},
                    {"edge", FilterFactories::CreateEdgeDetectionFilter},
                    {"blur", FilterFactories::CreateGaussianBlurFilter},
                    {"hex", FilterFactories::CreateHexagonalPixellateFilter},
                    {"cryst", FilterFactories::CreateCrystallizeFilter},
                    {"vign", FilterFactories::CreateVignettingFilter},
                    {"bulge", FilterFactories::CreateBulgeFilter},
                    {"grid", FilterFactories::CreateGridFilter}});
}

void Application::Run(int argc, const char* argv[]) {
    CmdArgParser parser;
    parser.Parse(argc, argv);
    auto descriptors = parser.GetFilterDescriptors();
    FilterPipeline fp = fpf_.CreateFilterPipeline(descriptors);
    Image image;
    image.Load(parser.GetInputFileName());
    fp.ApplyAll(image);
    image.Save(parser.GetOutputFileName());
}

BaseFilter* FilterFactories::CreateCropFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 2) {
        throw InvalidCmdArgument("Filter 'crop' takes 2 parameters, got " + std::to_string(fd.params.size()));
    }
    size_t width, height;
    try {
        width = std::stoull(fd.params[0]);
        height = std::stoull(fd.params[1]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameters of 'crop' must be integers");
    }
    if (width == 0 || height == 0) {
        throw InvalidCmdArgument("Parameters of 'crop' must be positive");
    }
    return new CropFilter(width, height);
}

BaseFilter* FilterFactories::CreateGrayscaleFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 0) {
        throw InvalidCmdArgument("Filter 'grayscale' takes 0 parameters, got " + std::to_string(fd.params.size()));
    }
    return new GrayscaleFilter();
}

BaseFilter* FilterFactories::CreateNegativeFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 0) {
        throw InvalidCmdArgument("Filter 'negative' takes 0 parameters, got " + std::to_string(fd.params.size()));
    }
    return new NegativeFilter();
}

BaseFilter* FilterFactories::CreateSharpeningFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 0) {
        throw InvalidCmdArgument("Filter 'sharpening' takes 0 parameters, got " + std::to_string(fd.params.size()));
    }
    return new SharpeningFilter();
}

BaseFilter* FilterFactories::CreateEdgeDetectionFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'edge detection' takes 1 parameter, got " + std::to_string(fd.params.size()));
    }
    double threshold;
    try {
        threshold = std::stod(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'edge detection' must be a floating point number");
    }
    return new EdgeDetectionFilter(threshold);
}

BaseFilter* FilterFactories::CreateGaussianBlurFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'gaussian blur' takes 1 parameter, got " + std::to_string(fd.params.size()));
    }
    // when processing each of 2 dimensions, (1-precision) of the pixel is calculated properly and
    // the rest is extrapolated for performance purposes
    double sigma, precision = 0.1;
    try {
        sigma = std::stod(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'gaussian blur' must be a floating point number");
    }
    if (sigma <= 0) {
        throw InvalidCmdArgument("Parameter of 'gaussian blur' must be positive");
    }
    return new GaussianBlurFilter(sigma, precision);
}

BaseFilter* FilterFactories::CreateHexagonalPixellateFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'hexagonal pixellate' takes 1 parameter, got " +
                                 std::to_string(fd.params.size()));
    }
    double a;
    try {
        a = std::stod(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'hexagonal pixellate' must be a floating point number");
    }
    if (a < 2) {
        throw InvalidCmdArgument("Parameter of 'hexagonal pixellate' must be at least 2");
    }
    return new HexagonalPixellateFilter(a);
}

BaseFilter* FilterFactories::CreateCrystallizeFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'crystallize' takes 1 parameter, got " + std::to_string(fd.params.size()));
    }
    int64_t n;
    try {
        n = std::stoll(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'crystallize' must be an integer");
    }
    if (n <= 0) {
        throw InvalidCmdArgument("Parameter of 'crystallize' must be positive");
    }
    return new CrystallizeFilter(n);
}

BaseFilter* FilterFactories::CreateVignettingFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'vignetting' takes 1 parameter, got " + std::to_string(fd.params.size()));
    }
    double alpha;
    try {
        alpha = std::stod(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'vignetting' must be a floating point number");
    }
    if (alpha <= 0) {
        throw InvalidCmdArgument("Parameter of 'vignetting' must be positive");
    }
    return new VignettingFilter(alpha);
}

BaseFilter* FilterFactories::CreateBulgeFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw InvalidCmdArgument("Filter 'bulge' takes 1 parameter, got " + std::to_string(fd.params.size()));
    }
    double alpha;
    try {
        alpha = std::stod(fd.params[0]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameter of 'bulge' must be a floating point number");
    }
    return new BulgeFilter(alpha);
}

BaseFilter* FilterFactories::CreateGridFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 2) {
        throw InvalidCmdArgument("Filter 'grid' takes 2 parameters, got " + std::to_string(fd.params.size()));
    }
    int64_t step, width;
    try {
        step = std::stoull(fd.params[0]);
        width = std::stoull(fd.params[1]);
    } catch (std::invalid_argument&) {
        throw InvalidCmdArgument("Parameters of 'grid' must be integers");
    }
    if (step == 0 || width == 0) {
        throw InvalidCmdArgument("Parameters of 'grid' must be positive");
    }
    return new GridFilter(step, width);
}
