#include "includes/utils.h"

// Get a map of tags and their frequencies from a comma-separated string
std::map<std::string, int> get_tags_from_string(const std::string& tags_str) {
    std::map<std::string, int> tags;
    std::stringstream ss(tags_str);
    std::string tag;
    while (std::getline(ss, tag, ',')) {
        tag.erase(remove_if(tag.begin(), tag.end(), isspace), tag.end()); // Trim spaces
        if (!tag.empty()) {
            tags[tag]++;
        }
    }
    return tags;
}