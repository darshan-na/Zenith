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
    title->SetText("Darshan's Personal Blog");
    channel->InsertEndChild(title);

    auto* link = doc.NewElement("link");
    link->SetText("https://www.darshanna.com");
    channel->InsertEndChild(link);

    auto* description = doc.NewElement("description");
    description->SetText("Darshan's personal blog posts");
    channel->InsertEndChild(description);

    auto* language = doc.NewElement("language");
        language->SetText("en-us");
        channel->InsertEndChild(language);

      auto* lastBuildDate = doc.NewElement("lastBuildDate");
        lastBuildDate->SetText(format_rfc822_date(std::time(nullptr)).c_str());
        channel->InsertEndChild(lastBuildDate);

    auto* ttl = doc.NewElement("ttl");
        ttl->SetText("60");  // cache duration in minutes
        channel->InsertEndChild(ttl);

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

        auto* item_guid = doc.NewElement("guid");
            item_guid->SetAttribute("isPermaLink", "true");
            item_guid->SetText(item.link.c_str());
            item_element->InsertEndChild(item_guid);
    }

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    FileUtils::write_file(output_path, printer.CStr());
}