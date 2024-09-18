#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <unordered_map>
#include "xxml.h"

//////////////////////////////
// XML CONTENT
//////////////////////////////

#pragma region XML CONTENT

xxml::builder::XmlContent::XmlContent(xxml::builder::XmlContent::Type type, int level)
    : type(type), level(level)
{
}

xxml::builder::XmlContent::~XmlContent()
{
}

xxml::builder::XmlContent::Type xxml::builder::XmlContent::get_type()
{
    return type;
}

int xxml::builder::XmlContent::get_level()
{
    return level;
}

void xxml::builder::XmlContent::align_tab(std::stringstream &xml)
{
    const int level = get_level();
    for (int i = 0; i < level; ++i)
    {
        xml << "\t";
    }
}

bool xxml::builder::XmlContent::validate_no_space(const std::string &input)
{
    for (char ch : input)
    {
        // 공백(스페이스), 탭(\t), 엔터(\n), 캐리지 리턴(\r) 검사
        if (std::isspace(ch))
        {
            return false; // 잘못된 문자가 발견되면 false 반환
        }
    }
    return true; // 문제가 없으면 true 반환
}

std::string xxml::builder::XmlContent::replace_to_entity_reference(const std::string &input)
{
    static std::unordered_map<std::string, std::string> entity_map = {
        // TODO: 유니코드
        {"<", "&lt;"},
        {">", "&gt;"},
        {"&", "&amp;"},
        {"'", "&apos;"},
        {"\"", "&quot;"}};

    std::string output;
    for (char c : input)
    {
        std::string s(1, c);
        // 특수 문자인지 확인하고 엔티티 참조로 변환
        if (entity_map.find(s) != entity_map.end())
        {
            output += entity_map[s];
        }
        else
        {
            output += c;
        }
    }
    return output;
}

#pragma endregion

//////////////////////////////
// TEXT
//////////////////////////////

#pragma region TEXT
xxml::builder::Text::Text(const char *text, int level)
    : xxml::builder::XmlContent(xxml::builder::XmlContent::Type::text, level), text(text)
{
}

xxml::builder::Text::~Text()
{
}

std::string xxml::builder::Text::build()
{
    std::stringstream this_xml;

    align_tab(this_xml);
    this_xml << text << "\n";

    return this_xml.str();
}
#pragma endregion

//////////////////////////////
// TAG
//////////////////////////////

#pragma region TAG

xxml::builder::Tag::Tag(std::shared_ptr<xxml::builder::Tag> parents, const char *name, xxml::builder::Tag::End end, int level)
    : XmlContent(xxml::builder::XmlContent::Type::tag, level), parents(parents), name(name), end(end)
{
}

xxml::builder::Tag::~Tag()
{
}

std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::tag(const char *name, xxml::builder::Tag::End end)
{
    if (this->end == xxml::builder::Tag::End::self_closing)
    {
        return nullptr;
    }

    if (!validate_no_space(name))
    {
        return nullptr;
    }

    const int next_level = get_level() + 1;

    std::shared_ptr<xxml::builder::Tag> new_tag = std::make_shared<xxml::builder::Tag>(shared_from_this(), name, end, next_level);
    children.push_back(new_tag);

    return new_tag;
}

std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::attribute(const char *attr, const char *value)
{
    if (!validate_no_space(attr))
    {
        return nullptr;
    }
    
    std::string replaced_text = replace_to_entity_reference(value);

    attributes.push_back({attr, replaced_text.c_str()});

    return shared_from_this();
}

std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::text(const char *value)
{
    std::string replaced_text = replace_to_entity_reference(value);
    const int next_level = get_level() + 1;
    
    std::shared_ptr<xxml::builder::Text> new_text = std::make_shared<xxml::builder::Text>(replaced_text.c_str(), next_level);
    children.push_back(new_text);

    return shared_from_this();
}

std::shared_ptr<xxml::builder::Tag> xxml::builder::Tag::close()
{
    return parents.lock();
}

std::string xxml::builder::Tag::build()
{
    std::stringstream this_xml;

    /**
     * "<tag_name "
     */
    align_tab(this_xml);
    this_xml << "<" << name;

    /**
     * "<tag_name attr1=val1 attr2=val2"
     */
    for (int i = 0; i < attributes.size(); ++i)
    {
        const std::pair<std::string, std::string> &attribute = attributes[i];
        this_xml << " " << attribute.first << "=" << "\"" << attribute.second << "\"";
    }

    /**
     * "<tag_name attr1=val1 attr2=val2/>"
     */
    if (end == xxml::builder::Tag::End::self_closing)
    {
        this_xml << "/>";
        return this_xml.str();
    }
    /**
     * "<tag_name attr1=val1 attr2=val2/>
     *  "
     */
    else
    {
        this_xml << ">\n";
    }

    /**
     * "<tag_name attr1=val1 attr2=val2/>
     *      texts
     *      children's tags"
     */
    for (std::shared_ptr<xxml::builder::XmlContent> child : children)
    {
        std::string child_xml = child->build();
        this_xml << child_xml;
    }

    /**
     * "<tag_name attr1=val1 attr2=val2 />
     *      children's tags
     *  </tag_name>"
     */
    align_tab(this_xml);
    this_xml << "</" << name << ">\n";

    return this_xml.str();
}

#pragma endregion

//////////////////////////////
// BUILDER
//////////////////////////////

#pragma region BUILDER
xxml::builder::Builder::Builder()
    : xxml::builder::Tag(nullptr, "builder", xxml::builder::Tag::End::regular, -1)
{
}

std::string xxml::builder::Builder::build()
{
    std::stringstream total_xml;
    for (int i = 0; i < children.size(); ++i)
    {
        std::shared_ptr<xxml::builder::XmlContent> child = children[i];
        std::string child_xml = child->build();
        total_xml << child_xml;
    }

    return total_xml.str();
}
#pragma endregion
