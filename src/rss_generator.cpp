#include "includes/rss_generator.h"
#include "includes/file_utils.h"
#include <tinyxml2.h>

void RSSGenerator::generate_rss(const std::vector<RSSItem>& items, const std::string& output_path) {
    tinyxml2::XMLDocument doc;
    auto* decl = doc.NewDeclaration();
    doc.InsertFirstChild(decl);

    auto* rss = doc.NewElement("rss");
    rss->SetAttribute("version", "2.0");
    doc.InsertEndChild(rss);

    auto* channel = doc.NewElement("channel");
    rss->InsertEndChild(channel);

    auto* title = doc.NewElement("title");
    title->SetText("My Personal Blog");
    channel->InsertEndChild(title);

    auto* link = doc.NewElement("link");
    link->SetText("https://yoursite.com");
    channel->InsertEndChild(link);

    auto* description = doc.NewElement("description");
    description->SetText("My personal blog posts");
    channel->InsertEndChild(description);

    for (const auto& item : items) {
        auto* item_element = doc.NewElement("item");
        channel->InsertEndChild(item_element);

        auto* item_title = doc.NewElement("title");
        item_title->SetText(item.title.c_str());
        item_element->InsertEndChild(item_title);

        auto* item_link = doc.NewElement("link");
        item_link->SetText(item.link.c_str());
        item_element->InsertEndChild(item_link);

        auto* item_description = doc.NewElement("description");
        item_description->SetText(item.description.c_str());
        item_element->InsertEndChild(item_description);

        auto* item_pub_date = doc.NewElement("pubDate");
        item_pub_date->SetText(item.pub_date.c_str());
        item_element->InsertEndChild(item_pub_date);
    }

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    FileUtils::write_file(output_path, printer.CStr());
}