#include "FileType.h"
#include "Specific/FileType_Lua.h"
#include "Specific/FileType_CSharp.h"
#include "Specific/FileType_Shader.h"

const std::unordered_map<std::string,
            std::function<std::unique_ptr<FileType>()>> FileType::creators = {
        {".lua", []() { return std::make_unique<FileTypeLua>(); }},
        {".cs", []() { return std::make_unique<FileTypeCSharp>(); }},
        {".hlsl", []() { return std::make_unique<FileTypeShader>(); }},
        {".hlsli", []() { return std::make_unique<FileTypeShader>(); }},
        {".compute", []() { return std::make_unique<FileTypeShader>(); }},
        {".cginc", []() { return std::make_unique<FileTypeShader>(); }},
        {".shader", []() { return std::make_unique<FileTypeShader>(); }},
            };

std::unique_ptr<FileType> FileType::GetFileType(const std::string& ext) {
    if (auto it = creators.find(ext); it != creators.end()) {
        return it->second();
    }

    return nullptr;
}