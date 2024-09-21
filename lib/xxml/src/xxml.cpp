#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <expat.h> // for validation
#include "xxml.h"

std::string xxml::builder::XmlDoc::build(const int base_level)
{
    std::vector<std::shared_ptr<xxml::builder::Tag>> tag_stack;
    std::shared_ptr<xxml::builder::Tag> recent_tag;
    std::stringstream ss;

#define ALIGN_TAB()                                         \
    for (int i = 0; i < base_level + tag_stack.size(); ++i) \
    {                                                       \
        ss << '\t';                                         \
    }

    for (std::shared_ptr<xxml::builder::XmlContent> content : contents)
    {
        tag_stack.empty() ? recent_tag = nullptr : recent_tag = tag_stack.back();
        xxml::builder::XmlContent::Type type = content->get_type();

        if (type == xxml::builder::XmlContent::Type::decl)
        {
            std::shared_ptr<xxml::builder::Declaration> xml_declaration = std::static_pointer_cast<xxml::builder::Declaration>(content);
            ss << "<?xml version=\'" << xml_declaration->get_version() << "\' encoding=\'" << xml_declaration->get_encoding() << "\' standalone=\'" << xml_declaration->get_standalone() << "\' ?>\n";
        }
        else if (type == xxml::builder::XmlContent::Type::tag)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            std::shared_ptr<xxml::builder::Tag> tag = std::static_pointer_cast<xxml::builder::Tag>(content);
            xxml::builder::TagType tag_type = tag->get_tag_type();

            ALIGN_TAB();

            ss << "<" << tag->get_name();

            if (tag_type == xxml::builder::TagType::regular || tag_type == xxml::builder::TagType::menual_closing)
            {
                tag_stack.push_back(tag);
            }
            else
            {
                ss << " />\n";
            }
        }
        else if (type == xxml::builder::XmlContent::Type::attr)
        {
            std::shared_ptr<xxml::builder::Attribute> attr = std::static_pointer_cast<xxml::builder::Attribute>(content);
            ss << " " << attr->get_name() << "=\"" << attr->get_value() << "\"";
        }
        else if (type == xxml::builder::XmlContent::Type::text)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            ALIGN_TAB();
            std::shared_ptr<xxml::builder::Text> text = std::static_pointer_cast<xxml::builder::Text>(content);
            ss << " " << text->get_text();
        }
        else if (type == xxml::builder::XmlContent::Type::close)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            tag_stack.pop_back();
            ALIGN_TAB();
            std::shared_ptr<xxml::builder::Close> close = std::static_pointer_cast<xxml::builder::Close>(content);

            xxml::builder::TagType recent_tag_type = recent_tag->get_tag_type();
            if (recent_tag_type == xxml::builder::TagType::regular)
            {
                ss << "</" << recent_tag->get_name() << ">\n";
            }
            else if (recent_tag_type == xxml::builder::TagType::menual_closing)
            {
                ss << "</" << recent_tag->get_name() << ">\n";
            }
        }
        else if (type == xxml::builder::XmlContent::Type::xml)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }
            std::shared_ptr<xxml::builder::XmlDoc> xml = std::static_pointer_cast<xxml::builder::XmlDoc>(content);
            ss << xml->build(tag_stack.size());
        }
        else if (type == xxml::builder::XmlContent::Type::pre_builded_xml)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            std::shared_ptr<xxml::builder::Text> text = std::static_pointer_cast<xxml::builder::Text>(content);
            std::string pre_builded_xml = std::string(text->get_text());

            std::string line = "";
            std::istringstream iss(pre_builded_xml);
            while (std::getline(iss, line))
            {
                ALIGN_TAB();
                ss << line << "\n";
            }
        }
    }

    if (recent_tag && recent_tag->is_open())
    {
        close_open_tag(ss, recent_tag);
    }

    return ss.str();
}

void xxml::builder::XmlDoc::close_open_tag(std::stringstream &ss, std::shared_ptr<xxml::builder::Tag> tag)
{
    xxml::builder::TagType tag_type = tag->get_tag_type();
    if (tag_type == xxml::builder::TagType::self_closing)
    {
        ss << " />\n";
    }
    else if (tag_type == xxml::builder::TagType::regular || tag_type == xxml::builder::TagType::menual_closing)
    {
        ss << " >\n";
    }

    tag->close_open_tag();
}

std::shared_ptr<xxml::builder::XmlDoc> xxml::get_builder()
{
    return std::make_shared<xxml::builder::XmlDoc>();
}
