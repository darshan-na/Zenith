#include "includes/markdown_parser.h"
#include "includes/page_generator.h"
#include "includes/rss_generator.h"
#include "includes/file_utils.h"
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

int main() {
    const std::string content_dir = "content";
    const std::string output_dir = "public";
    const std::string template_dir = "src/templates";
    const std::string assets_dir = "assets";

    fs::create_directories(output_dir);
    fs::create_directories(output_dir + "/blogs");
    fs::create_directories(output_dir + "/projects");

    FileUtils::copy_directory(assets_dir, output_dir + "/assets");

    PageGenerator page_gen(template_dir);
    
    std::string about_content = FileUtils::read_file(content_dir + "/about.md");
    std::string about_html = MarkdownParser::parse_markdown(about_content);
    page_gen.generate_page("about.html", {{"content", about_html}}, output_dir + "/index.html");

    std::vector<std::pair<std::string, std::string>> projects;
    for (const auto& entry : fs::directory_iterator(content_dir + "/projects")) {
        if (entry.path().extension() == ".md") {
            std::string md_content = FileUtils::read_file(entry.path().string());
            auto metadata = MarkdownParser::extract_front_matter(md_content);
            std::string html_content = MarkdownParser::parse_markdown(md_content);
            projects.emplace_back(metadata["title"], html_content);
            std::string slug = FileUtils::slugify(metadata["title"]);
            page_gen.generate_page("projects.html", {{"content", html_content}, {"title", metadata["title"]}}, 
                output_dir + "/projects/" + slug + ".html");
        }
    }
    page_gen.generate_page("projects.html", {{"content", page_gen.generate_project_list(projects)}}, 
        output_dir + "/projects/index.html");

    std::vector<RSSItem> rss_items;
    std::vector<std::pair<std::string, std::string>> posts;
    for (const auto& entry : fs::directory_iterator(content_dir + "/blogs")) {
        if (entry.path().extension() == ".md") {
            std::string md_content = FileUtils::read_file(entry.path().string());
            auto metadata = MarkdownParser::extract_front_matter(md_content);
            std::string html_content = MarkdownParser::parse_markdown(md_content);
            posts.emplace_back(metadata["title"], html_content);
            std::string slug = FileUtils::slugify(metadata["title"]);
            page_gen.generate_page("blogs.html", {{"content", html_content}, {"title", metadata["title"]}}, 
                output_dir + "/blogs/" + slug + ".html");
            rss_items.push_back({
                metadata["title"],
                metadata["description"],
                "https://yoursite.com/blogs/" + slug + ".html",
                metadata["date"]
            });
        }
    }
    page_gen.generate_page("blogs.html", {{"content", page_gen.generate_post_list(posts)}}, 
        output_dir + "/blogs/index.html");

    RSSGenerator::generate_rss(rss_items, output_dir + "/rss.xml");

    std::cout << "Static site generated successfully in " << output_dir << std::endl;
    return 0;
}