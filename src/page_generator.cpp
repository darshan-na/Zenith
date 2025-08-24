#include "includes/page_generator.h"
#include "includes/file_utils.h"
#include "includes/utils.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#define DefaultTitle "Darshan N A"

// Constructor
PageGenerator::PageGenerator(const std::string &template_dir) : template_dir_(template_dir) {}

// Load a template file
std::string PageGenerator::load_template(const std::string &template_name)
{
    return FileUtils::read_file(template_dir_ + "/" + template_name);
}

// Replace placeholders in the template with actual data
std::string PageGenerator::replace_placeholders(const std::string &template_content,
                                                const std::map<std::string, std::string> &data)
{
    std::string result = template_content;
    for (const auto &pair : data)
    {
        std::string placeholder = "{{" + pair.first + "}}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos)
        {
            result.replace(pos, placeholder.length(), pair.second);
            pos += pair.second.length();
        }
    }
    return result;
}

// Generate a page from a template - combines the base template with the page-specific content
void PageGenerator::generate_page(const std::string &template_name,
                                  const std::map<std::string, std::string> &data,
                                  const std::string &output_path)
{
    std::string base_template = load_template("base.html");
    std::string page_template = load_template(template_name);
    std::string content = replace_placeholders(page_template, data);
    std::map<std::string, std::string> base_data = {{"content", content}};
    base_data["title"] = data.count("title") ? data.at("title") : DefaultTitle;
    std::string final_content = replace_placeholders(base_template, base_data);
    FileUtils::write_file(output_path, final_content);
}

// Generate a HTML list of projects
std::string PageGenerator::generate_project_list(const std::vector<std::map<std::string, std::string>> &projects)
{
    std::string list;
    for (const auto &project : projects)
    {
        list += "<li><strong>" + project.at("date") + "</strong><br>" +
                "<a href='/projects/" + FileUtils::slugify(project.at("title")) + ".html'>" + project.at("title") + "</a><br>" +
                project.at("description") + "<br>" + "<span class='technologies_used'>Technologies/skills used: " + project.at("technologies") + "</span></li>";
    }
    return "<ul>" + list + "</ul>";
}

// Generate a HTML list of blog posts
std::string PageGenerator::generate_post_list(const std::vector<std::map<std::string, std::string>> &posts, bool for_tag)
{
    std::string list;
    if (posts.empty())
        return "<p>No posts available.</p>";

    // Sort posts by date descending
    auto sorted_posts = posts;
    std::sort(sorted_posts.begin(), sorted_posts.end(), [](const auto &a, const auto &b)
              { return a.at("date") > b.at("date"); });

    for (const auto &post : sorted_posts)
    {
        std::string date_str = post.at("date");
        std::string title = post.at("title");
        std::string description = post.at("description");
        std::string tags_html = "";
        if (!for_tag)
        {
            std::string tags_str = post.count("tags") ? post.at("tags") : "";
            std::map<std::string,int> tags = get_tags_from_string(tags_str);
            tags_html = generate_tag_list(tags);
        }
        list += "<li><strong>" + date_str + "</strong><br>" +
                "<a href='/blogs/" + FileUtils::slugify(title) + ".html'>" + title + "</a><br>" +
                description + "<br>" + tags_html  + "</li>";
    }
    return "<ul>" + list + "</ul>";
}

// Generate a HTML list of tags
std::string PageGenerator::generate_tag_list(const std::map<std::string,int> &tags, bool include_count)
{
    std::string list = "<ul class='tag-list'>";
    for (const auto &tag : tags)
    {
        std::string slug = FileUtils::slugify(tag.first);
        list += "<li><button class='tag-button' "
                "onclick=\"window.location.href='/blogs/tag/" +
                slug + ".html'\">" + (include_count ? tag.first + " (" + std::to_string(tag.second) + ")" : tag.first) + "</button></li>";
    }
    list += "</ul>";
    return list;
}
