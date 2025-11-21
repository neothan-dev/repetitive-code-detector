#include "common.h"
#include "FileType_Shader.h"

FileTypeShader::FileTypeShader() : FileType(".Shader") {
    this->FileTypeShader::ignoredLineHsSet = {
        getHash("{"),
        getHash("}"),
        getHash("#endif"),
    };
    
}

bool FileTypeShader::CheckPush(const std::string &line) {
    const std::string &striped = getStripedStr(line, "//");

    const int32_t stripedHs = getHash(striped);

    if (striped.empty() 
        || ignoredLineHsSet.count(stripedHs)) 
        return false;

    return true;
}

bool FileTypeShader::CheckSplitLine(const std::string &line) {
    // 去掉前后空格
    const std::string &trimmed = getStripedStr(line, "//");

    // Shader 函数声明常见形式：
    // [返回类型] 函数名(参数...) { 或 ;
    // 返回类型可能是 float4, int, void, Texture2D, struct名等
    static const std::regex funcPattern(
        R"(^\s*[\w<>,\[\]]+\s+\w+\s*\([^)]*\)\s*(?:\{|;)?\s*$)",
        std::regex::ECMAScript | std::regex::icase
    );

    // Shader struct 声明：
    // struct 名 { 或 ;
    static const std::regex structPattern(
        R"(^\s*struct\s+\w+\s*(?:\{|;)?\s*$)",
        std::regex::ECMAScript | std::regex::icase
    );

    return std::regex_match(trimmed, funcPattern) ||
           std::regex_match(trimmed, structPattern);
}

std::pair<bool, bool> FileTypeShader::CheckIgnoreSwitch(const std::string &line, const bool outsideStatus) {
    const std::string &striped = getStripedStr(line);
    if (striped.size() > 2 && striped.substr(0, 2) == "\\*" && outsideStatus == false) return std::pair<bool, bool> (true, true);
    if (striped.size() > 2 && striped.substr(0, 2) == "*\\" && outsideStatus == true) return std::pair<bool, bool> (false, true);
    
    return std::pair<bool, bool> (outsideStatus, false);
}