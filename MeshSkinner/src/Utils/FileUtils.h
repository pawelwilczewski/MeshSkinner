#pragma once

namespace FileUtils
{
    std::string ReadFile(const std::string &filepath);
    std::string FileParentPath(const std::string &path);
    std::string FileExtension(const std::string &path);
    std::vector<std::string> ListDirectory(const std::string &path);
}
