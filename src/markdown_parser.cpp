#include "includes/markdown_parser.h"
#include <cmark-gfm.h>
#include <regex>

// Parse the markdown content and convert it to HTML
std::string MarkdownParser::parse_markdown(const std::string& markdown) {
    auto content = markdown.c_str();
    cmark_node* doc = cmark_parse_document(content, markdown.size(), CMARK_OPT_DEFAULT);
    char* html = cmark_render_html(doc, CMARK_OPT_DEFAULT, nullptr);
    std::string result(html);
    cmark_node_free(doc);
    free(html);
    return result;
}

// Extract the metadata section from the markdown content
std::map<std::string, std::string> MarkdownParser::extract_front_matter(std::string& markdown) {
    std::map<std::string, std::string> metadata;
    std::regex front_matter_regex(R"(---\n([\s\S]*?)\n---\n)");
    std::smatch match;
    if (std::regex_search(markdown, match, front_matter_regex)) {
        std::string front_matter = match[1].str();
        markdown = match.suffix().str();
        std::regex key_value_regex(R"((\w+):\s*(.+))");
        auto begin = std::sregex_iterator(front_matter.begin(), front_matter.end(), key_value_regex);
        auto end = std::sregex_iterator();
        for (auto i = begin; i != end; ++i) {
            metadata[(*i)[1].str()] = (*i)[2].str();
        }
    }
    return metadata;
}