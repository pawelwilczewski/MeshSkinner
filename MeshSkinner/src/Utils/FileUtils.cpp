#include "pch.h"
#include "FileUtils.h"

std::string FileUtils::ReadFile(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file)
    {
        printf("Trying to read from non-existent file: %s\n", filepath.c_str());
        return "";
    }
    std::string result;
    file.seekg(0, std::ios::end);
    result.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&result[0], result.size());
    file.close();

    return result;
}

std::string FileUtils::FileParentPath(const std::string &path)
{
    std::filesystem::path p(path);
    if (p.has_parent_path())
        return p.parent_path().string();
    else
        return "";
}

std::string FileUtils::FileExtension(const std::string &path)
{
    std::filesystem::path p(path);
    if (p.has_extension())
        return p.extension().string();
    else
        return "";
}

std::vector<std::string> FileUtils::ListDirectory(const std::string &path)
{
    std::vector<std::string> files;

    auto p = std::filesystem::directory_iterator(path);
    for (const auto &file : p)
        if (!file.is_directory())
            files.push_back(file.path().string());
    return files;
}
