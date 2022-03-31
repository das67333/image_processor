cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2")

add_executable(image_processor
    application.cpp
    cmd_arg_parser.cpp
    exceptions.cpp
    image.cpp
    filter_crop.cpp
    filter_edge_detection.cpp
    filter_gaussian_blur.cpp
    filter_grayscale.cpp
    filter_negative.cpp
    filter_sharpening.cpp
    filter_hexagonal_pixellate.cpp
    filter_crystallize.cpp
    filter_vignetting.cpp
    filter_bulge.cpp
    filter_grid.cpp
    filter_pipeline.cpp
    filter_pipeline_factory.cpp
    image_processor.cpp
)

add_executable(test_image_processor
    cmd_arg_parser.cpp
    exceptions.cpp
    image.cpp
    filter_crop.cpp
    filter_edge_detection.cpp
    filter_gaussian_blur.cpp
    filter_grayscale.cpp
    filter_negative.cpp
    unity_tests.cpp
)
