#pragma once

#include "common.h"

class FileType {
public:
    FileType(const std::string& extension) : extension(extension) {}
    virtual ~FileType() = default;
    virtual bool CheckPush(const std::string &line) = 0;
    virtual bool CheckSplitLine(const std::string &line) = 0;
    virtual std::pair<bool, bool> CheckIgnoreSwitch(const std::string &line, const bool outsideStatus) = 0;

    virtual std::string getExtension() {
        return extension;
    }

    static std::unique_ptr<FileType> GetFileType(const std::string& ext);

protected:
    std::string extension;
    static const std::unordered_map<std::string,
            std::function<std::unique_ptr<FileType>()>> creators;
    
    std::set<int32_t> ignoredLineHsSet;
};