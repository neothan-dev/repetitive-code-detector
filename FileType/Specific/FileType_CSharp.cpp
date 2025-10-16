#include "common.h"
#include "FileType_CSharp.h"

bool FileTypeCSharp::CheckPush(const std::string &line) {
    const std::string &striped = GetStripedStr(line);

    if (striped.empty() || striped == "{" || striped == "}" || striped.size() >= 2 && striped.substr(0, 2) == "//") return false;

    return true;
}

bool FileTypeCSharp::CheckSplitLine(const std::string& line) {
    // 去掉前后空格
    std::string trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

    // 方法声明模式
    static const std::regex methodPattern(
        R"(^\s*(?:public|private|protected|internal)?\s*(?:static|virtual|override|sealed|async)?\s*[\w<>,\[\]\?]+\s+\w+\s*(?:<[^>]+>)?\s*\([^)]*\)\s*(?:where\s+\w+\s*:\s*[\w,]+)?\s*(?:\{|;)?\s*$)",
        std::regex::ECMAScript | std::regex::icase
    );

    // 类/结构体声明模式
    static const std::regex typePattern(
        R"(^\s*(?:public|private|protected|internal)?\s*(?:abstract|sealed|static)?\s*(class|struct)\s+\w+\s*(?:<[^>]+>)?(?:\s*:\s*[\w<>,\s]+)?\s*\{?\s*$)",
        std::regex::ECMAScript | std::regex::icase
    );

    // namespace 声明模式
    static const std::regex namespacePattern(
        R"(^\s*namespace\s+[\w\.]+\s*\{?\s*$)",
        std::regex::ECMAScript | std::regex::icase
    );

    return std::regex_match(trimmed, methodPattern)
        || std::regex_match(trimmed, typePattern)
        || std::regex_match(trimmed, namespacePattern);
}