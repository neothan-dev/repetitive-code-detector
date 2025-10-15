#include "common.h"
#include "ST.h"

ST::ST(std::vector<int32_t> arr) {
    this->n = arr.size() - 1;

    lg.resize(n + 1);
    lg[0] = -1;
    for (int32_t i = 1; i <= n; i++) {
        lg[i] = (i & (i - 1)) ? lg[i - 1] : lg[i - 1] + 1;
    }

    stMx.resize(n + 1, std::array<int32_t, Log + 5>());
    stMn.resize(n + 1, std::array<int32_t, Log + 5>());
    for (int32_t i = 1; i <= n; i++) {
        stMx[i][0] = arr[i];
        stMn[i][0] = arr[i];
    }
    for (int32_t j = 1; j <= Log; j++) {
        for (int32_t i = 1; i + (1LL << j) - 1 <= n; i++) {
            stMx[i][j] = std::max(stMx[i][j - 1], stMx[i + (1LL << (j - 1))][j - 1]);
            stMn[i][j] = std::min(stMn[i][j - 1], stMn[i + (1LL << (j - 1))][j - 1]);
        }
    }
}

int32_t ST::queryMax(int32_t l, int32_t r) {
    int32_t k = lg[r - l + 1];
    return std::max(stMx[l][k], stMx[r - (1LL << k) + 1][k]);
}

int32_t ST::queryMin(int32_t l, int32_t r) {
    int32_t k = lg[r - l + 1];
    return std::min(stMn[l][k], stMn[r - (1LL << k) + 1][k]);
}