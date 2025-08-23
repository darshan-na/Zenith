#ifndef RSS_GENERATOR_H
#define RSS_GENERATOR_H
#include <string>
#include <vector>

struct RSSItem {
    std::string title;
    std::string description;
    std::string link;
    std::string pub_date;
};

class RSSGenerator {
public:
    static void generate_rss(const std::vector<RSSItem>& items, const std::string& output_path);
};

#endif