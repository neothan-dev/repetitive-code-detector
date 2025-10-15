#pragma once

#include "common.h"
#include "SA.h"
#include "ST.h"

inline std::string GetStripedStr(const std::string &line) {
    std::string striped = "";
    for (auto ch : line) {
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            striped.push_back(ch);
        }
    }
    return striped;
}

inline std::string removeChinese(const std::string& utf8str) {
    std::string result;
    for (auto it = utf8str.begin(); it != utf8str.end(); ) {
        uint8_t c = *it;
        int32_t charLen = 0;
        if ((c & 0x80) == 0) charLen = 1;        // ASCII
        else if ((c & 0xE0) == 0xC0) charLen = 2; // 2字节字符
        else if ((c & 0xF0) == 0xE0) charLen = 3; // 中文常见范围
        else if ((c & 0xF8) == 0xF0) charLen = 4; // 4字节字符
        else { ++it; continue; }                 // 非法序列跳过

        // 非中文字符保留（ASCII或非中文多字节字符）
        if (charLen == 1) result.append(it, it+charLen);
        it += charLen;
    }
    return result;
}

inline void normalizeLineEndings(std::string& content) {
    std::string normalized;
    normalized.reserve(content.size());

    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i] == '\r') {
            // 判断是 \r\n (Windows) 还是单独的 \r (Old Mac)
            if (i + 1 < content.size() && content[i + 1] == '\n') {
                normalized += '\n';
                i++; // 跳过下一个字符
            } else {
                normalized += '\n';
            }
        } else {
            normalized += content[i];
        }
    }

    content = std::move(normalized);
}