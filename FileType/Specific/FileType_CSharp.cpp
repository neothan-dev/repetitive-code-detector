#include "common.h"
#include "FileType_CSharp.h"

FileTypeCSharp::FileTypeCSharp() : FileType(".cs") {
    this->FileTypeCSharp::ignoredLineHsSet = {
        getHash("{"),
        getHash("}"),
        getHash("*"),
        getHash("#endregion"),
        getHash("#endif"),
        getHash("#region"),

    };
}

bool FileTypeCSharp::CheckPush(const std::string &line) {
    const std::string &striped = getStripedStr(line, "//");

    const int32_t stripedHs = getHash(striped);

    if (striped.empty() 
        || ignoredLineHsSet.count(stripedHs)) 
        return false;

    return true;
}

bool FileTypeCSharp::CheckSplitLine(const std::string& line) {
    // 去掉前后空格
    const std::string &trimmed = getStripedStr(line, "//");

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

std::pair<bool, bool> FileTypeCSharp::CheckIgnoreSwitch(const std::string &line, const bool outsideStatus) {
    const std::string &striped = getStripedStr(line);
    if (striped.size() > 2 && striped.substr(0, 2) == "\\*" && outsideStatus == false) return std::pair<bool, bool> (true, true);
    if (striped.size() > 2 && striped.substr(0, 2) == "*\\" && outsideStatus == true) return std::pair<bool, bool> (false, true);
    
    return std::pair<bool, bool> (outsideStatus, false);
}