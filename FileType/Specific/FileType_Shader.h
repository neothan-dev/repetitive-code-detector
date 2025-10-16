#pragma once

#include "../FileType.h"

class FileTypeShader : public FileType {
public:
    FileTypeShader() : FileType(".Shader") {}
    bool CheckPush(const std::string &line) override;
    bool CheckSplitLine(const std::string &line) override;
};