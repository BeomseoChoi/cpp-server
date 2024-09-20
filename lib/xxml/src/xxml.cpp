#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <expat.h> // for validation
#include "xxml.h"

// //////////////////////////////
// // XML CONTENT
// //////////////////////////////

// #pragma region XML CONTENT

// xxml::builder::XmlContent::XmlContent(xxml::builder::XmlContent::Type type, int level)
//     : type(type), level(level)
// {
// }

// xxml::builder::XmlContent::~XmlContent()
// {
// }

// xxml::builder::XmlContent::Type xxml::builder::XmlContent::get_type()
// {
//     return type;
// }

// int xxml::builder::XmlContent::get_level()
// {
//     return level;
// }

// void xxml::builder::XmlContent::align_tab(std::stringstream &xml)
// {
//     const int level = get_level();
//     for (int i = 0; i < level; ++i)
//     {
//         xml << "\t";
//     }
// }

// bool xxml::builder::XmlContent::validate_no_space(const std::string &input)
// {
//     for (char ch : input)
//     {
//         // 공백(스페이스), 탭(\t), 엔터(\n), 캐리지 리턴(\r) 검사
//         if (std::isspace(ch))
//         {
//             return false; // 잘못된 문자가 발견되면 false 반환
//         }
//     }
//     return true; // 문제가 없으면 true 반환
// }

// std::string xxml::builder::XmlContent::replace_to_entity_reference(const std::string &input)
// {
//     static std::unordered_map<std::string, std::string> entity_map = {
//         {"<", "&lt;"},
//         {">", "&gt;"},
//         {"&", "&amp;"},
//         {"'", "&apos;"},
//         {"\"", "&quot;"}};

//     std::string output;
//     for (char c : input)
//     {
//         std::string s(1, c);
//         if (entity_map.find(s) != entity_map.end())
//         {
//             output += entity_map[s];
//         }
//         else
//         {
//             output += c;
//         }
//     }
//     return output;
// }

// #pragma endregion

// //////////////////////////////
// // XML DECLARATION
// //////////////////////////////

// #pragma region XML DECLARATION

// xxml::builder::Declaration::Declaration(const std::string &version, const std::string &encoding, const std::string &standalone, int level)
//     : XmlContent(Type::declaration, level), version(version), encoding(encoding), standalone(standalone) {}

// std::string xxml::builder::Declaration::build()
// {
//     std::stringstream xml;
//     // level 0
//     xml << "<?xml version=\"" << version << "\" encoding=\"" << encoding << "\" standalone=\"" << standalone << "\"?>\n";
//     return xml.str();
// }

// #pragma endregion

// //////////////////////////////
// // TAG
// //////////////////////////////

// #pragma region TAG

// xxml::builder::Tag::Tag(const std::string_view name)
//     : XmlContent(xxml::builder::XmlContent::Type::tag, level), builder(builder), parents(parents), name(name), end(end)
// {
// }
// xxml::builder::Tag::~Tag()
// {
// }

// std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::tag(const char *name, xxml::builder::Tag::End end)
// {
//     if (this->end == xxml::builder::Tag::End::self_closing)
//     {
//         return nullptr;
//     }

//     if (!validate_no_space(name))
//     {
//         return nullptr;
//     }

//     const int next_level = get_level() + 1;

//     std::shared_ptr<xxml::builder::Tag> new_tag = std::make_shared<xxml::builder::Tag>(builder, shared_from_this(), name, end, next_level);
//     children.push_back(new_tag);

//     return new_tag;
// }

// std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::attribute(const char *attr, const char *value)
// {
//     if (!validate_no_space(attr))
//     {
//         return nullptr;
//     }

//     std::string replaced_text = replace_to_entity_reference(value);

//     attributes.push_back({attr, replaced_text.c_str()});

//     return shared_from_this();
// }

// std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::text(const char *value)
// {
//     std::string replaced_text = replace_to_entity_reference(value);
//     const int next_level = get_level() + 1;

//     std::shared_ptr<xxml::builder::Text> new_text = std::make_shared<xxml::builder::Text>(replaced_text.c_str(), next_level);
//     children.push_back(new_text);

//     return shared_from_this();
// }

// std::shared_ptr<xxml::builder::XmlContent> xxml::builder::Tag::close()
// {
//     return parents.lock();
// }

// std::string xxml::builder::Tag::build()
// {
//     std::stringstream this_xml;

//     /**
//      * "<tag_name "
//      */
//     align_tab(this_xml);
//     this_xml << "<" << name;

//     /**
//      * "<tag_name attr1=val1 attr2=val2"
//      */
//     for (int i = 0; i < attributes.size(); ++i)
//     {
//         const std::pair<std::string, std::string> &attribute = attributes[i];
//         this_xml << " " << attribute.first << "=" << "\"" << attribute.second << "\"";
//     }

//     /**
//      * "<tag_name attr1=val1 attr2=val2/>"
//      */
//     if (end == xxml::builder::Tag::End::self_closing)
//     {
//         this_xml << "/>";
//         return this_xml.str();
//     }
//     /**
//      * "<tag_name attr1=val1 attr2=val2/>
//      *  "
//      */
//     else
//     {
//         this_xml << ">\n";
//     }

//     /**
//      * "<tag_name attr1=val1 attr2=val2/>
//      *      texts
//      *      children's tags"
//      */
//     for (std::shared_ptr<xxml::builder::XmlContent> child : children)
//     {
//         std::string child_xml = child->build();
//         this_xml << child_xml;
//     }

//     /**
//      * "<tag_name attr1=val1 attr2=val2 />
//      *      children's tags
//      *  </tag_name>"
//      */
//     align_tab(this_xml);
//     this_xml << "</" << name << ">\n";

//     return this_xml.str();
// }

// #pragma endregion

// //////////////////////////////
// // TEXT
// //////////////////////////////

// #pragma region TEXT
// xxml::builder::Text::Text(const char *text, int level)
//     : xxml::builder::XmlContent(xxml::builder::XmlContent::Type::text, level), text(text)
// {
// }

// xxml::builder::Text::~Text()
// {
// }

// std::string xxml::builder::Text::build()
// {
//     std::stringstream this_xml;

//     align_tab(this_xml);
//     this_xml << text << "\n";

//     return this_xml.str();
// }
// #pragma endregion

// //////////////////////////////
// // BUILDER
// //////////////////////////////

// #pragma region BUILDER
// xxml::builder::XmlDoc::XmlDoc()
//     : xxml::builder::XmlContent(xxml::builder::XmlContent::Type::doc, -1)
// {
// }

// std::shared_ptr<xxml::builder::Tag> xxml::builder::XmlDoc::tag(const char *name, xxml::builder::Tag::End end)
// {
//     const int next_level = get_level() + 1;

//     std::shared_ptr<xxml::builder::Tag> new_tag = std::make_shared<xxml::builder::Tag>(shared_from_this(), shared_from_this(), name, end, next_level);

//     contents.push_back(new_tag);

//     return new_tag;
// }

// std::shared_ptr<xxml::builder::XmlDoc> xxml::builder::XmlDoc::declaration(const std::string &version, const std::string &encoding, const std::string &standalone)
// {
//     const int next_level = get_level() + 1;

//     xml_declaration = std::make_shared<xxml::builder::Declaration>(version, encoding, standalone, next_level);

//     return shared_from_this();
// }

// std::string xxml::builder::XmlDoc::build()
// {
//     std::stringstream total_xml;
//     total_xml << xml_declaration->build();
//     for (int i = 0; i < contents.size(); ++i)
//     {
//         std::shared_ptr<xxml::builder::Tag> child = contents[i];
//         std::string child_xml = child->build();
//         total_xml << child_xml;
//     }

//     return total_xml.str();
// }
// #pragma endregion

// bool xxml::validate(const std::string &xml)
// {
//     static XML_Parser parser = XML_ParserCreate(NULL);
//     if (!parser)
//     {
//         std::cerr << "Couldn't allocate memory for parser\n";
//         return 1;
//     }

//     // 콜백 함수 설정
//     XML_SetElementHandler(parser, nullptr, nullptr);

//     // XML 데이터를 파싱
//     if (XML_Parse(parser, xml.c_str(), xml.length(), XML_TRUE) == XML_STATUS_ERROR)
//     {
//         std::cerr << "XML parse error: " << XML_ErrorString(XML_GetErrorCode(parser)) << std::endl;
//         return false;
//     }

//     // 파서 해제
//     // XML_ParserFree(parser);

//     return true;
// }

std::string xxml::builder::XmlDoc::build()
{
    int level = -1;
    xxml::builder::TagType recent_tag_type;
    std::shared_ptr<xxml::builder::XmlContent> recent_content = nullptr;
    std::shared_ptr<xxml::builder::Tag> recent_tag = nullptr;

    std::stringstream ss;
    for (std::shared_ptr<xxml::builder::XmlContent> content : contents)
    {
        xxml::builder::XmlContent::Type type = content->get_type();

        if (type == xxml::builder::XmlContent::Type::decl)
        {
            std::shared_ptr<xxml::builder::Declaration> xml_declaration = std::static_pointer_cast<xxml::builder::Declaration>(content);
            ss << "<?xml version=\'" << xml_declaration->get_version() << "\' encoding=\'" << xml_declaration->get_encoding() << "\' standalone=\'" << xml_declaration->get_standalone() << "\' ?>\n";

            recent_content = xml_declaration;
        }
        else if (type == xxml::builder::XmlContent::Type::tag)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            std::shared_ptr<xxml::builder::Tag> tag = std::static_pointer_cast<xxml::builder::Tag>(content);
            xxml::builder::TagType tag_type = tag->get_tag_type();

            for (int i = 0; i < level; ++i)
            {
                ss << '\t';
            }

            if (tag_type == xxml::builder::TagType::regular || tag_type == xxml::builder::TagType::menual_closing)
            {
                ++level;
            }

            ss << "<" << tag->get_name();
            recent_tag_type = tag_type;

            recent_content = tag;
            recent_tag = tag;
        }
        else if (type == xxml::builder::XmlContent::Type::attr)
        {
            std::shared_ptr<xxml::builder::Attribute> attr = std::static_pointer_cast<xxml::builder::Attribute>(content);
            ss << " " << attr->get_name() << "=\"" << attr->get_value() << "\"";

            recent_content = attr;
        }
        else if (type == xxml::builder::XmlContent::Type::text)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            std::shared_ptr<xxml::builder::Text> text = std::static_pointer_cast<xxml::builder::Text>(content);
            ss << " " << text->get_text();

            recent_content = text;
        }
        else if (type == xxml::builder::XmlContent::Type::close)
        {
            if (recent_tag && recent_tag->is_open())
            {
                close_open_tag(ss, recent_tag);
            }

            std::shared_ptr<xxml::builder::Close> close = std::static_pointer_cast<xxml::builder::Close>(content);

            xxml::builder::TagType recent_tag_type = recent_tag->get_tag_type();
            if (recent_tag_type == xxml::builder::TagType::regular)
            {
                --level;

                ss << "</" << recent_tag->get_name() << ">\n";
            }
            else if (recent_tag_type == xxml::builder::TagType::menual_closing)
            {
                --level;

                ss << "</" << recent_tag->get_name() << ">\n";
            }

            recent_content = close;
        }
    }

    if (recent_tag->is_open())
    {
        close_open_tag(ss, recent_tag);
    }

    return ss.str();
}

void xxml::builder::XmlDoc::close_open_tag(std::stringstream& ss, std::shared_ptr<xxml::builder::Tag> tag)
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