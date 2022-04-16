cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2")

add_executable(image_processor
    src/application.cpp
    src/cmd_arg_parser.cpp
    src/exceptions.cpp
    src/image.cpp
    src/filter_crop.cpp
    src/filter_edge_detection.cpp
    src/filter_gaussian_blur.cpp
    src/filter_grayscale.cpp
    src/filter_negative.cpp
    src/filter_sharpening.cpp
    src/filter_hexagonal_pixellate.cpp
    src/filter_crystallize.cpp
    src/filter_vignetting.cpp
    src/filter_bulge.cpp
    src/filter_grid.cpp
    src/filter_pipeline.cpp
    src/filter_pipeline_factory.cpp
    src/image_processor.cpp
)

add_executable(test_image_processor
    src/cmd_arg_parser.cpp
    src/exceptions.cpp
    src/image.cpp
    src/filter_crop.cpp
    src/filter_edge_detection.cpp
    src/filter_gaussian_blur.cpp
    src/filter_grayscale.cpp
    src/filter_negative.cpp
    src/unity_tests.cpp
)
