#pragma once

#include "../FileType.h"

class FileTypeCSharp : public FileType {
public:
    FileTypeCSharp();
    bool CheckPush(const std::string &line) override;
    bool CheckSplitLine(const std::string &line) override;
    std::pair<bool, bool> CheckIgnoreSwitch(const std::string &line, const bool outsideStatus) override;
};