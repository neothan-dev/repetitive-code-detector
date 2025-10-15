#pragma once

#include "common.h"

constexpr int32_t Log = 20;

class ST {
public:
    ST(std::vector<int32_t> arr);
    
    int32_t queryMax(int32_t l, int32_t r);
    int32_t queryMin(int32_t l, int32_t r);

    int32_t n;
    std::vector<int32_t> lg;
    std::vector<std::array<int32_t, Log + 5>> stMx, stMn;
};
