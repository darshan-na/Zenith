#include "includes/page_generator.h"
#include "includes/file_utils.h"
#include <fstream>

PageGenerator::PageGenerator(const std::string& template_dir) : template_dir_(template_dir) {}

std::string PageGenerator::load_template(const std::string& template_name) {
    return FileUtils::read_file(template_dir_ + "/" + template_name);
}

std::string PageGenerator::replace_placeholders(const std::string& template_content, 
                                              const std::map<std::string, std::string>& data) {
    std::string result = template_content;
    for (const auto& pair : data) {
        std::string placeholder = "{{" + pair.first + "}}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), pair.second);
            pos += pair.second.length();
        }
    }
    return result;
}

void PageGenerator::generate_page(const std::string& template_name, 
                                const std::map<std::string, std::string>& data, 
                                const std::string& output_path) {
    std::string base_template = load_template("base.html");
    std::string page_template = load_template(template_name);
    std::string content = replace_placeholders(page_template, data);
    std::map<std::string, std::string> base_data = {{"content", content}};
    if (data.find("title") != data.end()) {
        base_data["title"] = data.at("title");
    } else {
        base_data["title"] = "My Site";  // Default title
    }
    std::string final_content = replace_placeholders(base_template, base_data);
    FileUtils::write_file(output_path, final_content);
}

std::string PageGenerator::generate_project_list(const std::vector<std::pair<std::string, std::string>>& projects) {
    std::string list;
    for (const auto& project : projects) {
        list += "<div class='card mb-3'><div class='card-body'>";
        list += "<h3><a href='/projects/" + FileUtils::slugify(project.first) + ".html'>" + project.first + "</a></h3>";
        list += project.second + "</div></div>";
    }
    return list;
}

std::string PageGenerator::generate_post_list(const std::vector<std::pair<std::string, std::string>>& posts) {
    std::string list;
    for (const auto& post : posts) {
        list += "<div class='card mb-3'><div class='card-body'>";
        list += "<h3><a href='/blogs/" + FileUtils::slugify(post.first) + ".html'>" + post.first + "</a></h3>";
        list += post.second + "</div></div>";
    }
    return list;
}