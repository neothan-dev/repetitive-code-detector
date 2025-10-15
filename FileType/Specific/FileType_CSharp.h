#pragma once

#include "../FileType.h"

class FileTypeCSharp : public FileType {
public:
    FileTypeCSharp() : FileType(".cs") {}
    bool CheckSplitLine(const std::string& line) override;
    bool CheckPush(const std::string &line) override;
};