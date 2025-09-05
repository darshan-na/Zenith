#pragma once
#include <string>
#include <vector>
#include <ctime> 

struct RSSItem
{
    std::string title;
    std::string description;
    std::string link;
    std::string pub_date;
};

class RSSGenerator
{
public:
    static void generate_rss(const std::vector<RSSItem> &items, const std::string &output_path);
    static std::string format_rfc822_date(time_t t) {
    tm tm{};
    char buf[128];
#ifdef _WIN32
    gmtime_s(&tm, &t);  // Windows safe version
#else
    gmtime_r(&t, &tm);  // POSIX safe version
#endif
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);
    return std::string(buf);
}
};