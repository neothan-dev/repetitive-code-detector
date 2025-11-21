#include "common.h"
#include "FileType_Lua.h"


FileTypeLua::FileTypeLua() : FileType(".lua") {
    this->FileTypeLua::ignoredLineHsSet = {
        getHash("end"),
        getHash("collectgarbage(\"collect\")"),
    };
    
}

bool FileTypeLua::CheckPush(const std::string &line) {
    const std::string &striped = getStripedStr(line, "--");

    const int32_t stripedHs = getHash(striped);

    if (striped.size() <= 2
        || ignoredLineHsSet.count(stripedHs)
        || striped.size() >= 2 && striped.substr(0, 2) == "..")
        return false;

    bool onlyDigitAndBigBracketFlag = true;
    for (auto &x : striped) {
        if (!(x >= '0' && x <= '9' || x == ',' || x == '{' || x == '}')) {
            onlyDigitAndBigBracketFlag = false;
        }
    }
    if (onlyDigitAndBigBracketFlag) {
        return false;
    }

    auto checkIsOnlyVariable = [](const std::string &s) -> bool {
        // Lua 关键字列表
        static const std::unordered_set<std::string> keywords = {
            "and","break","do","else","elseif","end","false","for","function",
            "goto","if","in","local","not","or","repeat","return","then",
            "true","until","while"
        };

        // 正则匹配：首字符字母或下划线，后续字母/数字/下划线
        static const std::regex pattern(R"(^[A-Za-z_][A-Za-z0-9_]*,?$)");

        if (!std::regex_match(s, pattern)) {
            return false;
        }

        // 检查是否是关键字
        if (s.size() > 10) {
            return true;
        } else {
            std::string str = s;
            if (str.back() == ',') str.pop_back();
            return keywords.find(str) == keywords.end();
        }
    };
    if (striped.size() < 30) {
        std::string str = "";
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
                str = line.substr(i, line.size() - i);
                break;
            }
        }
        int idx = str.find('--');
        if (idx != -1) {
            str = str.substr(0, idx);
        }
        while (!str.empty() && (str.back() == ' ' || str.back() == '\t' || str.back() == '\n')){
            str.pop_back();
        }
        if (checkIsOnlyVariable(str)) return false;
    }

    return true;
}

bool FileTypeLua::CheckSplitLine(const std::string &line) {
    const std::string &striped = getStripedStr(line, "--");
    
    if (striped.size() > 8 && striped.substr(0, 8) == "function") {
        return true;
    } else {
        return false;
    }
}

std::pair<bool, bool> FileTypeLua::CheckIgnoreSwitch(const std::string &line, const bool outsideStatus) {
    const std::string &striped = getStripedStr(line);
    if (striped == "--[[" && outsideStatus == false) return std::pair<bool, bool> (true, true);
    if (striped == "]]" && outsideStatus == true) return std::pair<bool, bool> (false, true);

    return std::pair<bool, bool> (outsideStatus, false);
}