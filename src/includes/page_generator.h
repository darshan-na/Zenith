#ifndef PAGE_GENERATOR_H
#define PAGE_GENERATOR_H
#include <string>
#include <map>
#include <vector>

class PageGenerator {
public:
    PageGenerator(const std::string& template_dir);
    void generate_page(const std::string& template_name, 
                     const std::map<std::string, std::string>& data, 
                     const std::string& output_path);
    std::string generate_project_list(const std::vector<std::pair<std::string, std::string>>& projects);
    std::string generate_post_list(const std::vector<std::pair<std::string, std::string>>& posts);

private:
    std::string template_dir_;
    std::string load_template(const std::string& template_name);
    std::string replace_placeholders(const std::string& template_content, 
                                   const std::map<std::string, std::string>& data);
};

#endif