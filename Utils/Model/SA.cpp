#include "common.h"
#include "SA.h"

SA::SA(std::vector<int32_t> str) {
    this->n = str.size() - 1;
    this->str = str;
    this->sa.resize(n + 1);
    this->rk.resize(n + 1);
    this->fstsa.resize(n + 1);
    this->lstrk.resize(n * 2 + 1);
    this->rem.resize(n + 1);
    ori.eb(-INF);
    for (int32_t i = 1; i <= n; i++) {
        ori.eb(str[i]);
    }
    sort(ori.begin(), ori.end());
    ori.erase(unique(ori.begin(), ori.end()), ori.end());

    getSA();
    getHeight();
}

int32_t SA::id(int32_t x) {
    return std::lower_bound(ori.begin(), ori.end(), x) - ori.begin();
}

void SA::getSA() {
    m = (int32_t)ori.size() - 1;
    cnt.resize(n + 1);
    auto Csort = [&]() -> void {
        std::fill(cnt.begin(), cnt.end(), 0LL);
        for (int32_t i = 1; i <= n; i++) cnt[rem[i] = rk[fstsa[i]]]++;
        for (int32_t i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (int32_t i = n; i; i--) sa[cnt[rem[i]]--] = fstsa[i];
    };
    for (int32_t i = 1; i <= n; i++) {
        fstsa[i] = i, rk[i] = id(str[i]);
    }
    Csort();
    int32_t i, tot;
    for (int64_t w = 1; ; w <<= 1, m = tot) {
        for (i = n, tot = 0; i > n - w; i--) fstsa[++tot] = i;
        for (i = 1; i <= n; i++) if (sa[i] > w) fstsa[++tot] = sa[i] - w;
        Csort();
        for (i = 1; i <= n; i++) lstrk[i] = rk[i];
        for (i = 1, tot = 0; i <= n; i++) rk[sa[i]] = (lstrk[sa[i]] == lstrk[sa[i - 1]] && lstrk[sa[i] + w] == lstrk[sa[i - 1] + w]) ? tot : ++tot;
        if (tot == n) break;
    }
}

void SA::getHeight() {      //height[rk[i]] >= height[rk[i - 1]] - 1;
    height.resize(n + 1);
    for (int32_t i = 1, k = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (k) k--;
        while (i + k <= n && sa[rk[i] - 1] + k <= n && str[i + k] == str[sa[rk[i] - 1] + k]) k++;
        height[rk[i]] = k;
    }
}