#include "common.h"
#include "FileType_Shader.h"

bool FileTypeShader::CheckPush(const std::string &line) {
    const std::string &striped = GetStripedStr(line);

    if (striped.empty() || striped == "{" || striped == "}" || striped.size() >= 2 && striped.substr(0, 2) == "//") return false;

    return true;
}

bool FileTypeShader::CheckSplitLine(const std::string &line) {
    // 去掉前后空格
    std::string trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

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