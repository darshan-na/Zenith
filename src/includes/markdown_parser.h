#ifndef MARKDOWN_PARSER_H
#define MARKDOWN_PARSER_H
#include <string>
#include <map>
#include <vector>

class MarkdownParser {
public:
    static std::string parse_markdown(const std::string& markdown);
    static std::map<std::string, std::string> extract_front_matter(std::string& markdown);
};

#endif