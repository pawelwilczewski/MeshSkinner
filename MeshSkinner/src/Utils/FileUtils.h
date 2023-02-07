#pragma once

class FileUtils
{
public:
    static std::string ReadFile(const std::string &filepath);
    static std::string FileParentPath(const std::string &path);
    static std::string FileExtension(const std::string &path);
    static std::vector<std::string> ListDirectory(const std::string &path);
};
