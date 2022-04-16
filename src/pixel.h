#pragma once

#include <cstddef>
#include <cstdint>

struct Pixel {
    static constexpr size_t NUM_PRIMARY_COLORS = 3;

    uint8_t data[NUM_PRIMARY_COLORS] = {0};
} __attribute__((packed));
