#pragma once

#include "../FileType.h"

class FileTypeLua : public FileType {
public:
    FileTypeLua() : FileType(".lua") {}
    bool CheckSplitLine(const std::string &line) override;
    bool CheckPush(const std::string &line) override;
};