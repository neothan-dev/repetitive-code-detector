#include "common.h"
#include "FileType_Lua.h"

bool FileTypeLua::CheckPush(const std::string &line) {
    std::string striped = GetStripedStr(line);

    if (striped.empty() || striped == "end"|| striped.size() >= 2 && striped.substr(0, 2) == "--") return false;

    return true;
}

bool FileTypeLua::CheckSplitLine(const std::string &line) {
    if (line.size() > 8 && line.substr(0, 8) == "function") {
        return true;
    } else {
        return false;
    }
}