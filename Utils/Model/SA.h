#pragma once

#include "common.h"

class SA {
public:
    SA(std::vector<int32_t> str);
    
    int32_t id(int32_t x);
    void getSA();
    void getHeight();

    int32_t n, m;
    std::vector<int32_t> str;
    std::vector<int32_t> ori;
    std::vector<int32_t> sa, rk, fstsa, lstrk, rem, cnt;
    std::vector<int32_t> height;
};
