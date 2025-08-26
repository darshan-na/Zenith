#include "includes/markdown_parser.h"
#include "includes/page_generator.h"
#include "includes/rss_generator.h"
#include "includes/file_utils.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

int yaml_parse(const std::string &file_path, std::map<std::string, std::string> &data)
{
    YAML::Node config = YAML::LoadFile(file_path);
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        data[it->first.as<std::string>()] = it->second.as<std::string>();
    }
    return 0;
}

int main()
{
    // parse the yaml file
    std::map<std::string, std::string> config;
    yaml_parse("config.yaml", config);

    std::string content_dir = config["content"];
    std::string output_dir = config["output"];
    std::string template_dir = config["template"];
    std::string assets_dir = config["assets"];

    // remove the output/target directory if existing
    fs::remove_all(output_dir);
    // create the output/target directory structure
    fs::create_directories(output_dir);
    fs::create_directories(output_dir + "/blogs");
    fs::create_directories(output_dir + "/projects");
    fs::create_directories(output_dir + "/blogs/tag");

    // copy the css and scripts.js file
    FileUtils::copy_directory(assets_dir, output_dir + "/assets");

    // Initialize the page generator
    PageGenerator page_gen(template_dir);

    // AboutMe page
    std::string aboutme_content = FileUtils::read_file(content_dir + "/aboutme.md");
    std::string aboutme_html = MarkdownParser::parse_markdown(aboutme_content);
    page_gen.generate_page("aboutme.html", {{"content", aboutme_html}}, output_dir + "/index.html");

    // Generate Individual projects page
    std::vector<std::map<std::string, std::string>> projects;
    for (const auto &entry : fs::directory_iterator(content_dir + "/projects"))
    {
        if (entry.path().extension() == ".md")
        {
            std::string md_content = FileUtils::read_file(entry.path().string());
            auto metadata = MarkdownParser::extract_front_matter(md_content);
            std::string html_content = MarkdownParser::parse_markdown(md_content);
            metadata["content"] = html_content;
            projects.push_back(metadata);
            std::string slug = FileUtils::slugify(metadata["title"]);
            page_gen.generate_page("projects.html", {{"content", html_content}, {"title", metadata["title"]}},
                                   output_dir + "/projects/" + slug + ".html");
        }
    }
    // Generate projects/index.html
    page_gen.generate_page("projects_index.html", {{"content", page_gen.generate_project_list(projects)}},
                           output_dir + "/projects/index.html");

    // Generate blog pages
    std::vector<RSSItem> rss_items;
    std::vector<std::map<std::string, std::string>> all_posts;
    std::map<std::string, int> tag_count;

    for (const auto &entry : fs::directory_iterator(content_dir + "/blogs"))
    {
        if (entry.path().extension() == ".md")
        {
            std::string md_content = FileUtils::read_file(entry.path().string());
            auto metadata = MarkdownParser::extract_front_matter(md_content);
            std::string html_content = MarkdownParser::parse_markdown(md_content);
            std::string slug = FileUtils::slugify(metadata["title"]);
            std::map<std::string, std::string> post_data = metadata;
            post_data["content"] = html_content;
            post_data["slug"] = slug;
            all_posts.push_back(post_data);

            // Generate individual blog page
            page_gen.generate_page("blogs.html", {{"content", html_content}, {"title", metadata["title"]}},
                                   output_dir + "/blogs/" + slug + ".html");

            // RSS
            rss_items.push_back({metadata["title"],
                                 metadata["description"],
                                 "https://yoursite.com/blogs/" + slug + ".html",
                                 metadata["date"]});

            // Collect tags
            if (metadata.count("tags"))
            {
                std::string tags_str = metadata["tags"];
                std::stringstream ss(tags_str);
                std::string tag;
                while (std::getline(ss, tag, ','))
                {
                    tag.erase(remove_if(tag.begin(), tag.end(), isspace), tag.end()); // Trim spaces
                    tag_count[tag]++;
                }
                post_data["tags"] = tags_str;
            }
        }
    }

    // Generate blogs/index.html with grouped list and tag list
    std::string tag_list = page_gen.generate_tag_list(tag_count, true);
    std::string grouped_list = page_gen.generate_post_list(all_posts, false);
    page_gen.generate_page("blogs_index.html", {{"frequent_tags", tag_list}, {"content", grouped_list}},
                           output_dir + "/blogs/index.html");

    // Generate tag pages
    for (const auto &tag : tag_count)
    {
        std::vector<std::map<std::string, std::string>> tagged_posts;
        for (const auto &post : all_posts)
        {
            if (post.count("tags") && post.at("tags").find(tag.first) != std::string::npos)
            {
                tagged_posts.push_back(post);
            }
        }
        std::string tag_content = page_gen.generate_post_list(tagged_posts, true);
        page_gen.generate_page("tags.html", {{"content", tag_content}, {"tag", tag.first}},
                               output_dir + "/blogs/tag/" + FileUtils::slugify(tag.first) + ".html");
    }

    // Generate RSS
    RSSGenerator::generate_rss(rss_items, output_dir + "/rss.xml");

    std::cout << "Static site generated successfully in " << output_dir << std::endl;
    return 0;
}