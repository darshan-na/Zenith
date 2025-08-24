#pragma once
#include <string>
#include <filesystem>

class FileUtils {
public:
    static std::string read_file(const std::string& path);
    static void write_file(const std::string& path, const std::string& content);
    static void copy_directory(const std::string& src, const std::string& dst);
    static std::string slugify(const std::string& text);
};