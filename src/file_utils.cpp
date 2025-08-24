#include "includes/file_utils.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

// Read the contents of a file
std::string FileUtils::read_file(const std::string& path) {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// Write the contents to a file
void FileUtils::write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

// Copy a directory
void FileUtils::copy_directory(const std::string& src, const std::string& dst) {
    fs::create_directories(dst);
    fs::copy(src, dst, fs::copy_options::recursive);
}

// Slugify a string 
// A slug is URL friendly version of a string
// It converts the string to lowercase, replaces spaces with hyphens, and removes invalid characters(only alphanumeric characters are allowed)
std::string FileUtils::slugify(const std::string& text) {
    std::string slug = text;
    std::transform(slug.begin(), slug.end(), slug.begin(), [](unsigned char c) { return std::tolower(c); });
    std::replace(slug.begin(), slug.end(), ' ', '-');
    slug.erase(std::remove_if(slug.begin(), slug.end(), [](unsigned char c) { 
        return !std::isalnum(c) && c != '-'; 
    }), slug.end());
    return slug;
}