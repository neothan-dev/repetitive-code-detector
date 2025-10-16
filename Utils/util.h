#pragma once

#include "common.h"
#include "SA.h"
#include "ST.h"

inline int64_t max(int64_t a, int64_t b) {
    return a > b ? a : b;
}

inline int64_t min(int64_t a, int64_t b) {
    return a < b ? a : b;
}

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

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif
inline std::string executeCommandAndRead(const std::string& command) {
    std::string result;
    char buffer[100000];
    
    FILE* pipe = POPEN(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("执行命令失败");
    }
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    int32_t status = PCLOSE(pipe);
    if (status == -1) {
        throw std::runtime_error("关闭管道失败");
    }
    
    return result;
}

inline std::string getSVNInfo(const std::string &filepath) {
    static std::map<std::string, std::string> rem;
    if (rem.count(filepath)) {
        return rem[filepath];
    } else {
        return rem[filepath] = executeCommandAndRead("svn blame -v " + filepath);
    }
}

inline std::string checkAndExtractSVNInfo(const std::string &line, const bool is_svn) {
    if (!is_svn) {
        return "";
    }
    std::string buf;
    int32_t flag = 0;
    for (int32_t i = 0; i < line.size(); i++) { 
        if (line[i] == ')') {
            return buf;
        } else if (line[i] == '+') {
            flag = 1;
        }

        if (!flag) {
            buf.push_back(line[i]);
        }
    }

    return "";
}

inline std::string fitString(const std::string &str, int num) {
    std::string ret = str;
    while (ret.size() < num) {
        ret.push_back(' ');
    }
    
    return ret;
}