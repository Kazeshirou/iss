#pragma once

#include <cstddef>


struct statistic_t {
    size_t empty_sequences{0};
    size_t steps{0};
    size_t operations{0};
    size_t read{0};
    size_t write{0};
};